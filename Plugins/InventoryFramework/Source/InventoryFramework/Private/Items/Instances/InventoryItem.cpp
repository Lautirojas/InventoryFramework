
#include "Items/Instances/InventoryItem.h"
#include "Net/UnrealNetwork.h"
#include "Items/Definitions/InventoryItemDefinition.h"

void UInventoryItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UInventoryItem, StackAmount);
}

void UInventoryItem::Initialize(
	UInventoryItemDefinition* InDefinition,
	int32 InitialStackAmount)
{
	if (!IsValid(InDefinition))
	{
		return;
	}

	Definition = InDefinition;
	StackAmount = FMath::Max(InitialStackAmount, 0);
}

void UInventoryItem::AddToStack(int32 Amount)
{
	if (Amount <= 0)
	{
		return;
	}

	StackAmount += Amount;
}

void UInventoryItem::RemoveFromStack(int32 Amount)
{
	if (Amount <= 0)
	{
		return;
	}

	StackAmount -= Amount;

	StackAmount = FMath::Max(0, StackAmount);
}

const UInventoryItemDefinition* UInventoryItem::GetInventoryDefinition() const
{
	return GetDefinitionAs<UInventoryItemDefinition>();
}

bool UInventoryItem::HasDefinition(
	const UInventoryItemDefinition* ItemDefinition) const
{
	return Definition == ItemDefinition;
}

bool UInventoryItem::CanMergeWithDefinition(
	const UInventoryItemDefinition* ItemDefinition) const
{
	if (!IsValid(ItemDefinition))
	{
		return false;
	}

	return Definition == ItemDefinition &&
		!IsStackFull();
}

int32 UInventoryItem::GetMaxStackSize() const
{
	const UInventoryItemDefinition* InventoryDefinition =
		GetDefinitionAs<UInventoryItemDefinition>();

	if (!InventoryDefinition)
	{
		return 0;
	}

	return InventoryDefinition->MaxStackSize;
}

int32 UInventoryItem::GetRemainingStackSpace() const
{
	return FMath::Max(
		0,
		GetMaxStackSize() - StackAmount);
}

bool UInventoryItem::IsStackFull() const
{
	return StackAmount >= GetMaxStackSize();
}

bool UInventoryItem::CanMergeWith(
	const UInventoryItem* OtherItem) const
{
	return IsValid(OtherItem) &&
		CanMergeWithDefinition(
			OtherItem->GetDefinitionAs<UInventoryItemDefinition>());
}

bool UInventoryItem::IsEmpty() const
{
	return StackAmount <= 0;
}

int32 UInventoryItem::MergeFrom(UInventoryItem* OtherItem)
{
	if (!CanMergeWith(OtherItem))
	{
		return 0;
	}

	const int32 SpaceLeft =
		GetRemainingStackSpace();

	const int32 AmountToMove =
		FMath::Min(
			OtherItem->StackAmount,
			SpaceLeft);

	AddToStack(AmountToMove);

	OtherItem->RemoveFromStack(AmountToMove);

	return AmountToMove;
}