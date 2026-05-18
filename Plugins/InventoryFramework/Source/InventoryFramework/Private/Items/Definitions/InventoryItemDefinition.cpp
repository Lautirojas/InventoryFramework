

#include "Items/Definitions/InventoryItemDefinition.h"
#include "Items/Instances/InventoryItem.h"

UInventoryItemDefinition::UInventoryItemDefinition()
{
	InstanceClass = UInventoryItem::StaticClass();
}
