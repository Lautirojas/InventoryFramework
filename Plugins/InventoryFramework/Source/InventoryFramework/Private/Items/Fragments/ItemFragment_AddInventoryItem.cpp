
#include "Items/Fragments/ItemFragment_AddInventoryItem.h"
#include "Items/Definitions/InventoryItemDefinition.h"
#include "Components/InventoryComponent.h"

void UItemFragment_AddInventoryItem::OnItemAcquired(AActor* Buyer) const
{
	if (!Buyer)
		return;

	UInventoryComponent* InventoryC = Buyer->GetComponentByClass<UInventoryComponent>();

	if (InventoryC && ItemToAdd)
	{
		InventoryC->AddItem(ItemToAdd, 1);
	}
}
