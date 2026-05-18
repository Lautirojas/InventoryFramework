
#pragma once

#include "CoreMinimal.h"
#include "ItemFragment.h"
#include "ItemFragment_AddInventoryItem.generated.h"

class UInventoryItemDefinition;

UCLASS()
class INVENTORYFRAMEWORK_API UItemFragment_AddInventoryItem : public UItemFragment
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory Framework|Inventory")
	TObjectPtr<UInventoryItemDefinition> ItemToAdd; // The item definition of the item to add to the inventory, using TObjectPtr for safe memory management and garbage collection

	virtual void OnItemAcquired(AActor* Buyer) const override; // Override the OnItemAcquired function to add the specified item to the buyer's inventory when the item is acquired
};