

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

	// The class of the world actor that represents this item when it's in the world (e.g., dropped on the ground), using TSubclassOf for safe type checking and editor integration
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory Framework|Inventory")
	TSubclassOf<AActor> WorldActorClass = nullptr;

	// The mesh that represents this item when it's in the world (e.g., dropped on the ground), using TObjectPtr for safe memory management and garbage collection. This can be either a static mesh or a skeletal mesh, as specified by the AllowedClasses metadata.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory Framework|Inventory", meta = (AllowedClasses = "/Script/Engine.StaticMesh,/Script/Engine.SkeletalMesh"))
	TObjectPtr<UStreamableRenderAsset> WorldMesh;
	
	// The maximum number of items that can be stacked together in the inventory. If MaxStackNumber is greater than 1, then multiple instances of this item can be combined into a single stack in the inventory, up to the specified maximum. If MaxStackNumber is 1, then each instance of this item will occupy its own slot in the inventory and cannot be stacked with other instances.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory Framework|Inventory", meta = (ClampMin = "1"))
	int32 MaxStackNumber = 1;

};
