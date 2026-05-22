#pragma once

#include "CoreMinimal.h"
#include "ItemInstance.h"
#include "InventoryItem.generated.h"

UCLASS(BlueprintType, EditInlineNew)
class INVENTORYFRAMEWORK_API UInventoryItem : public UItemInstance
{
	GENERATED_BODY()

public:


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory Framework|Inventory")
	int StackAmount = 0; // The amount of items in the stack 
};
