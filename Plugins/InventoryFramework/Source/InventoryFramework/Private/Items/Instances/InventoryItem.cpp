
#include "Items/Instances/InventoryItem.h"
#include "Net/UnrealNetwork.h"

void UInventoryItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UInventoryItem, StackAmount);
}
