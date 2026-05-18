



#include "Components/InventoryComponent.h"
#include "Items/Definitions/InventoryItemDefinition.h"
#include "Items/Instances/InventoryItem.h"

#include "InventoryFrameworklog.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

#pragma region Helpers

int UInventoryComponent::GetSize() const
{
	return Size;
}

const TArray<UInventoryItem*> UInventoryComponent::GetAllItems() const
{
	return Items;
}

#pragma endregion Helpers

#pragma region Inventory

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
	// Remove items matching the definition until the amount is reached
	int RemainingAmount = Amount;

	for (auto ItemItr = Items.CreateIterator(); ItemItr; ++ItemItr)
	{
		UInventoryItem* Item = *ItemItr;
		if (Item && Item->Definition == ItemDefinition)
		{
			if (Item->StackAmount > RemainingAmount)
			{
				Item->StackAmount -= RemainingAmount;
				if (Item->StackAmount == 0)
					ItemItr.RemoveCurrent();

				// EVENT to notify that the inventory has changed
				OnInventoryChanged.Broadcast();
				return true;

			}
			else
			{
				// EVENT to notify that the inventory has changed
				OnInventoryChanged.Broadcast();

				RemainingAmount -= Item->StackAmount;
				ItemItr.RemoveCurrent();
			}
		}
	}
	return false;
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
	// Validate input parameters

	if (!ItemDefinition)
	{
		UE_LOG(LogInventoryFramework, Error, TEXT("AddItem failed: ItemDefinition is null"));
		return false;
	}

	if (Amount <= 0)
	{
		UE_LOG(LogInventoryFramework, Warning, TEXT("AddItem failed: Invalid amount"));
		return false;
	}

	if (ItemDefinition->MaxStackNumber <= 0) {
		UE_LOG(LogInventoryFramework, Warning, TEXT("AddItem failed: Invalid MaxStackNumber in ItemDefinition"));
		return false;
	}

	// Continue with adding the item to the inventory

	// Set the item amount to add
	int NewAmount = Amount;
	// Index for iterating through the inventory
	int InventoryIndex = 0;

	// Check if there is a stack of the same item definition that is not full
	for (; InventoryIndex < Items.Num(); ++InventoryIndex)
	{
		// If we found a stack of the same item definition that is not full, add to it
		UInventoryItem* Item = Items[InventoryIndex];
		if (Item && Item->Definition == ItemDefinition && Item->StackAmount < ItemDefinition->MaxStackNumber)
		{
			// If there is still amount to add, set the amount to add to the stack, and reduce the amount to add by the amount added to the stack
			const int SpaceLeft = ItemDefinition->MaxStackNumber - Item->StackAmount;
			// The amount to add to the stack is the minimum of the remaining amount to add and the space left in the stack	
			const int AmountToAdd = FMath::Min(NewAmount, SpaceLeft);

			// Add the amount to the stack and reduce the amount to add by the amount added to the stack
			Item->StackAmount += AmountToAdd;
			NewAmount -= AmountToAdd;

			// If we have added all the items, return true
			if (NewAmount <= 0)
			{
				return true;
			}
		}

	}

	// Insert rest of elements into empty slots

	// If there are still items to add and there is space in the inventory, put them in new slots
	while (NewAmount > 0 && Items.Num() < Size)
	{
		// Create a new item and add it to the inventory
		UInventoryItem* Item = NewObject<UInventoryItem>();
		Item->Definition = ItemDefinition;
		Item->StackAmount = FMath::Min(NewAmount, ItemDefinition->MaxStackNumber);
		NewAmount -= Item->StackAmount;
		Items.Emplace(Item);
	}

	// Index for iterating through the inventory
	int index = 0;

	// If there are still item amount to add and slots nullptr, fill them with the amount of items left
	while (NewAmount > 0 && index < InventoryIndex)
	{
		// If the slot is empty, add a new item
		UInventoryItem* Item = Items[index];
		if (!Item)
		{
			// Create a new item and add it to the inventory
			Item = NewObject<UInventoryItem>(this);
			Item->Definition = ItemDefinition;
			Item->StackAmount = FMath::Min(NewAmount, ItemDefinition->MaxStackNumber);
			NewAmount -= Item->StackAmount;
			Items[index] = Item;
		}

		index++;
	}

	// EVENT to notify that the inventory has changed
	OnInventoryChanged.Broadcast();

	return NewAmount == 0;
}

bool UInventoryComponent::RemoveItemAt(int ItemIndex)
{
	if (ItemIndex >= Items.Num() || !Items[ItemIndex])
		return false;

	Items[ItemIndex] = nullptr;

	while (!Items.IsEmpty() && !Items.Last())
	{
		Items.Pop();
	}

	// EVENT to notify that the inventory has changed
	OnInventoryChanged.Broadcast();

	return true;
}

#pragma endregion Inventory

