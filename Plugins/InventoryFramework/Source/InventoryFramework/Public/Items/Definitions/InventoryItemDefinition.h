

#pragma once

#include "CoreMinimal.h"
#include "ItemDefinition.h"
#include "InventoryItemDefinition.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class INVENTORYFRAMEWORK_API UInventoryItemDefinition : public UItemDefinition
{
	GENERATED_BODY()
	
public:

	UInventoryItemDefinition();

	// Properties

	// The World Actor of the item
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory Framework|Inventory Item")
	TSubclassOf<AActor> WorldActorClass = nullptr;

	// The World visual of the item 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory Framework|Inventory Item", meta = (AllowedClasses = "/Script/Engine.StaticMesh,/Script/Engine.SkeletalMesh"))
	TObjectPtr<UStreamableRenderAsset> WorldVisual;
	
	// Maximum amount allowed per stack.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory Framework|Inventory Item", meta = (ClampMin = "1"))
	int32 MaxStackSize = 1;

};
