
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Items/Fragments/ItemFragment.h"
#include "ItemDefinition.generated.h"

// Forward Declarations
class UItemInstance;
class UAbilitySystemComponent;
class UItemFragment_GrantCard;

UCLASS(BlueprintType)
class INVENTORYFRAMEWORK_API UItemDefinition : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:

	// Properties

	// The fragments that define the behavior and properties of the item
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = "Inventory Framework|ItemDefinition")
	TArray<TObjectPtr<UItemFragment>> Fragments; // An array of item fragments that define the behavior and properties of the item, using TObjectPtr for safe memory management and garbage collection

	// The class of the item instance that will be created when this item is acquired
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory Framework|ItemDefinition")
	TSubclassOf<UItemInstance> InstanceClass; // The class of the item instance that will be created when this item is acquired, using TSubclassOf for safe type checking and editor integration

	// Display properties for the item
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory Framework|ItemDefinition")
	FText Name; // The display name of the item, using FText for localization support

	// The icon that represents the item in the UI
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory Framework|ItemDefinition")
	TObjectPtr<UTexture2D> Icon = nullptr; // The icon that represents the item in the UI

	// A description of the item, which can be used in tooltips or other UI elements
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory Framework|ItemDefinition")
    FText Description;

// Functions can be Override and Implement in Child Classes

    virtual UTexture2D* GetIcon() const
    {
        return Icon;
    }

    virtual FText GetDisplayName() const
    {
        return Name;
    }

    void OnItemAcquired(AActor* Buyer) const;

	// Template function to find a fragment of a specific class in the Fragments array

    template <typename T>
    const T* FindFragmentByClass() const
    {
        for (UItemFragment* Fragment : Fragments)
        {
            if (const T* TypedFragment = Cast<T>(Fragment))
            {
                return TypedFragment;
            }
        }
        return nullptr;
    }
};