



#include "Components/InventoryComponent.h"
#include "Items/Definitions/InventoryItemDefinition.h"
#include "Items/Instances/InventoryItem.h"
#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"
#include "InventoryFrameworklog.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);
}

// Replication setup
void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UInventoryComponent, Items);
}

bool UInventoryComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (UInventoryItem* Item : Items)
	{
		if (IsValid(Item))
		{
			WroteSomething |= Channel->ReplicateSubobject(Item, *Bunch, *RepFlags);
		}
	}

	return WroteSomething;
}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
	Items.SetNum(Size);
}

#pragma region InventoryQueries

int UInventoryComponent::GetSize() const
{
	return Size;
}

const TArray<UInventoryItem*>& UInventoryComponent::GetAllItems() const
{
	check(this);

	return Items;
}

bool UInventoryComponent::IsInventoryFull() const
{
	// Inventory is full if there are no free slots left
	return GetFreeSlotCount() <= 0;
}

bool UInventoryComponent::HasFreeSlot() const
{
	// Check if there is at least one free slot
	return GetFreeSlotCount() > 0;
}

int UInventoryComponent::GetFreeSlotCount() const
{
	int FreeSlots = 0;

	// Count nullptr slots
	for (const UInventoryItem* Item : Items)
	{
		if (!Item)
		{
			++FreeSlots;
		}
	}

	// Add remaining slots if inventory array is smaller than Size
	FreeSlots += Size - Items.Num();

	return FreeSlots;
}

bool UInventoryComponent::ContainsItem(UInventoryItemDefinition* ItemDefinition) const
{
	if (!IsValid(ItemDefinition))
	{
		return false;
	}

	for (const UInventoryItem* Item : Items)
	{
		if (Item && Item->Definition == ItemDefinition)
		{
			return true;
		}
	}

	return false;
}

bool UInventoryComponent::HasSpaceForItem(UInventoryItemDefinition* ItemDefinition, int Amount) const
{
	if (!IsValid(ItemDefinition) || Amount <= 0)
	{
		return false;
	}

	int RemainingAmount = Amount;

	// First try stacking into existing stacks
	for (const UInventoryItem* Item : Items)
	{
		if (CanStackItem(Item, ItemDefinition))
		{
			RemainingAmount -= GetRemainingStackSpace(Item);

			if (RemainingAmount <= 0)
			{
				return true;
			}
		}
	}

	// Then calculate how many items fit into free slots
	const int MaxStack = ItemDefinition->MaxStackNumber;
	const int FreeSlots = GetFreeSlotCount();

	const int CapacityFromFreeSlots = FreeSlots * MaxStack;

	return CapacityFromFreeSlots >= RemainingAmount;
}

#pragma endregion InventoryQueries

#pragma region InventoryFunctions

	#pragma region CoreFunctions

		bool UInventoryComponent::FindItemByDefinition(UInventoryItemDefinition* ItemDefinition, int& Index) const
		{
			// Validate input parameters

			if (!ItemDefinition)
			{
				return false;
			}

			// Loop through the inventory items to find a match with the provided item definition

			for (Index = 0; Index < Items.Num(); ++Index)
			{
				if (Items[Index] && Items[Index]->Definition == ItemDefinition)
				{
					return true;
				}
			}
			return false;
		}

		bool UInventoryComponent::RemoveItemAmountByDefinition(UInventoryItemDefinition* ItemDefinition, int Amount)
		{
			if (!ensure(ItemDefinition))
			{
				UE_LOG(LogInventoryFramework, Error, TEXT("RemoveItemAmountByDefinition failed: ItemDefinition is null"));
				return false;
			}

			if (Amount <= 0)
			{
				UE_LOG(LogInventoryFramework, Warning, TEXT("RemoveItemAmountByDefinition failed: Invalid amount"));
				return false;
			}

			int RemainingAmount = Amount;

			for (int Index = 0; Index < Items.Num() && RemainingAmount > 0; ++Index)
			{
				UInventoryItem* Item = Items[Index];

				if (!IsValid(Item))
				{
					continue;
				}

				if (Item->Definition != ItemDefinition)
				{
					continue;
				}

				// Remove from current stack

				const int AmountToRemove = FMath::Min(RemainingAmount, Item->StackAmount);

				Item->StackAmount -= AmountToRemove;
				RemainingAmount -= AmountToRemove;

				// Remove empty stacks

				if (Item->StackAmount <= 0)
				{
					Items[Index] = nullptr;
				}
			}

			// Remove trailing empty slots

			while (!Items.IsEmpty() && !IsValid(Items.Last()))
			{
				Items.Pop();
			}

			if (RemainingAmount != 0)
			{
				UE_LOG(LogInventoryFramework, Warning, TEXT("RemoveItemAmountByDefinition could not remove full amount"));
				return false;
			}

			NotifyInventoryChanged();

			return true;
		}

		int UInventoryComponent::GetItemAmountByDefinition(UInventoryItemDefinition* ItemDefinition) const
		{
			// Count items matching the definition
			int Amount = 0;
			for (const auto Item : Items)
			{
				if (Item && Item->Definition == ItemDefinition)
				{
					Amount += Item->StackAmount;
				}
			}
			return Amount;
		}

		bool UInventoryComponent::AddItem(UInventoryItemDefinition* ItemDefinition, int Amount)
		{
			if (!ensure(ItemDefinition))
			{
				UE_LOG(LogInventoryFramework, Error, TEXT("AddItem failed: ItemDefinition is null"));
				return false;
			}

			if (Amount <= 0)
			{
				UE_LOG(LogInventoryFramework, Warning, TEXT("AddItem failed: Invalid amount"));
				return false;
			}

			if (!ensure(ItemDefinition->MaxStackNumber > 0))
			{
				UE_LOG(LogInventoryFramework, Warning, TEXT("AddItem failed: Invalid MaxStackNumber in ItemDefinition"));
				return false;
			}

			if (Size <= 0)
			{
				UE_LOG(LogInventoryFramework, Warning, TEXT("AddItem failed: Inventory size is invalid"));
				return false;
			}

			// Continue with adding items to the inventory

			int RemainingAmount = Amount;

			// First try stacking into existing stacks

			for (UInventoryItem* Item : Items)
			{
				if (CanStackItem(Item, ItemDefinition))
				{
					const int SpaceLeft = GetRemainingStackSpace(Item);
					const int AmountToAdd = FMath::Min(RemainingAmount, SpaceLeft);

					Item->StackAmount += AmountToAdd;
					RemainingAmount -= AmountToAdd;

					// if the remaining amount is 0 or less, we are done and can exit early
					if (RemainingAmount <= 0)
					{
						NotifyInventoryChanged();
						return true;
					}
				}
			}

			// If we couldn't stack all items, check if there is enough free space to add new stacks
			// 
			// Create new stacks in empty slots until we have added the entire amount or run out of space in the inventory

			while (RemainingAmount > 0)
			{
				const int EmptySlotIndex = FindFirstEmptySlot();

				if (EmptySlotIndex == INDEX_NONE)
				{
					break;
				}

				const int StackAmount =
					FMath::Min(RemainingAmount, ItemDefinition->MaxStackNumber);

				UInventoryItem* NewItem =
					CreateInventoryItem(ItemDefinition, StackAmount);

				if (!ensure(NewItem))
				{
					return false;
				}

				RemainingAmount -= StackAmount;

				Items[EmptySlotIndex] = NewItem;
			}

			NotifyInventoryChanged();

			return RemainingAmount == 0;
		}

		bool UInventoryComponent::RemoveItemAt(int ItemIndex)
		{
			if (!IsValidSlotIndex(ItemIndex))
			{
				UE_LOG(LogInventoryFramework, Warning, TEXT("RemoveItemAt failed: Invalid slot index"));
				return false;
			}

			if (!IsValid(Items[ItemIndex]))
			{
				UE_LOG(LogInventoryFramework, Warning, TEXT("RemoveItemAt failed: No item slot at the specified index"));
				return false;
			}

			Items[ItemIndex] = nullptr;

			NotifyInventoryChanged();

			return true;
		}

	#pragma endregion CoreFunctions

	#pragma region InventoryDragDropFunctions

		bool UInventoryComponent::MoveItem(int FromIndex, int ToIndex)
		{
			if (!IsValidSlotIndex(FromIndex) ||
				!IsValidSlotIndex(ToIndex))
			{
				return false;
			}

			if (FromIndex == ToIndex)
			{
				return false;
			}

			UInventoryItem* SourceItem = Items[FromIndex];
			UInventoryItem* TargetItem = Items[ToIndex];

			if (!IsValid(SourceItem))
			{
				return false;
			}

			// STACKING
			if (CanMergeStacks(SourceItem, TargetItem))
			{
				const UInventoryItemDefinition* InventoryDefinition =
					Cast<UInventoryItemDefinition>(TargetItem->Definition);

				if (!InventoryDefinition)
				{
					return false;
				}

				const int MaxStack =
					InventoryDefinition->MaxStackNumber;

				const int SpaceLeft =
					MaxStack - TargetItem->StackAmount;

				const int AmountToMove =
					FMath::Min(SourceItem->StackAmount, SpaceLeft);

				TargetItem->StackAmount += AmountToMove;
				SourceItem->StackAmount -= AmountToMove;

				// remove source stack if empty
				if (SourceItem->StackAmount <= 0)
				{
					Items[FromIndex] = nullptr;
				}
			}
			else
			{
				// SWAP
				Items.Swap(FromIndex, ToIndex);
			}

			NotifyInventoryChanged();

			return true;
		}

		bool UInventoryComponent::TransferItemTo(UInventoryComponent* TargetInventory, int SourceIndex, int TargetIndex)
		{
			if (!IsValid(TargetInventory))
			{
				return false;
			}

			if (!IsValidSlotIndex(SourceIndex))
			{
				return false;
			}

			UInventoryItem* SourceItem = Items[SourceIndex];

			if (!IsValid(SourceItem))
			{
				return false;
			}

			// SAME INVENTORY
			if (TargetInventory == this)
			{
				return MoveItem(SourceIndex, TargetIndex);
			}

			// target slot exists?
			if (!TargetInventory->Items.IsValidIndex(TargetIndex))
			{
				return false;
			}

			UInventoryItem* TargetItem =
				TargetInventory->Items[TargetIndex];

			// STACK INTO TARGET
			if (TargetInventory->CanMergeStacks(
				SourceItem,
				TargetItem))
			{
				const UInventoryItemDefinition* InventoryDefinition =
					Cast<UInventoryItemDefinition>(TargetItem->Definition);

				if (!InventoryDefinition)
				{
					return false;
				}

				const int MaxStack =
					InventoryDefinition->MaxStackNumber;

				const int SpaceLeft =
					MaxStack - TargetItem->StackAmount;

				const int AmountToMove =
					FMath::Min(SourceItem->StackAmount, SpaceLeft);

				TargetItem->StackAmount += AmountToMove;
				SourceItem->StackAmount -= AmountToMove;

				if (SourceItem->StackAmount <= 0)
				{
					Items[SourceIndex] = nullptr;
				}
			}
			else
			{
				// SWAP BETWEEN INVENTORIES
				TargetInventory->Items[TargetIndex] = SourceItem;
				Items[SourceIndex] = TargetItem;
			}

			NotifyInventoryChanged();
			TargetInventory->NotifyInventoryChanged();

			return true;
		}


	#pragma endregion InventoryDragDropFunctions

#pragma endregion InventoryFunctions



#pragma region InternalHelpers

UInventoryItem* UInventoryComponent::CreateInventoryItem(UInventoryItemDefinition* ItemDefinition, int StackAmount)
{
	if (!ensure(ItemDefinition))
	{
		return nullptr;
	}

	UInventoryItem* NewItem = NewObject<UInventoryItem>(this);

	if (!ensure(NewItem))
	{
		return nullptr;
	}

	NewItem->Definition = ItemDefinition;
	NewItem->StackAmount = StackAmount;

	return NewItem;
}

int UInventoryComponent::FindFirstEmptySlot() const
{
	for (int i = 0; i < Items.Num(); ++i)
	{
		if (!IsValid(Items[i]))
		{
			return i;
		}
	}

	return INDEX_NONE;
}

bool UInventoryComponent::CanStackItem(const UInventoryItem* Item, const UInventoryItemDefinition* Definition) const
	{
		return Item &&
			Definition &&
			Item->Definition == Definition &&
			Item->StackAmount < Definition->MaxStackNumber;
	}

int UInventoryComponent::GetRemainingStackSpace(const UInventoryItem* Item) const
	{
		if (!Item || !Item->Definition)
		{
			return 0;
		}

		const UInventoryItemDefinition* InventoryDefinition = Cast<UInventoryItemDefinition>(Item->Definition);

		if (!InventoryDefinition)
		{
			return 0;
		}

		return InventoryDefinition->MaxStackNumber - Item->StackAmount;
	}

void UInventoryComponent::OnRep_Items()
{
	NotifyInventoryChanged();
}

void UInventoryComponent::NotifyInventoryChanged() const
{
	OnInventoryChanged.Broadcast();
}

bool UInventoryComponent::IsValidSlotIndex(int Index) const
{
	return Items.IsValidIndex(Index);
}

bool UInventoryComponent::CanMergeStacks(UInventoryItem* SourceItem, UInventoryItem* TargetItem) const
{
	if (!IsValid(SourceItem) || !IsValid(TargetItem))
	{
		return false;
	}

	if (SourceItem->Definition != TargetItem->Definition)
	{
		return false;
	}

	const UInventoryItemDefinition* InventoryDefinition =
		Cast<UInventoryItemDefinition>(TargetItem->Definition);

	if (!InventoryDefinition)
	{
		return false;
	}

	return TargetItem->StackAmount <
		InventoryDefinition->MaxStackNumber;
}

#pragma endregion InternalHelpers