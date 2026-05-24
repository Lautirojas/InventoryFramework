
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Items/Fragments/ItemFragment.h"
#include "ItemDefinition.generated.h"

// Forward Declarations
class UItemInstance;

UCLASS(BlueprintType)
class INVENTORYFRAMEWORK_API UItemDefinition : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:

	UItemDefinition();

	// Properties

	// The fragments that define the behavior and properties of the item
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = "Inventory Framework|Item Definition")
	TArray<TObjectPtr<UItemFragment>> Fragments; 

	// The class of the item instance that will be created when this item is acquired
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory Framework|Item Definition") //meta = (AllowAbstract = false)
    TSubclassOf<UItemInstance> InstanceClass;

	// Display properties for the item

    // Name
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory Framework|Item Definition")
	FText Name;

	// Icon
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory Framework|Item Definition")
	TObjectPtr<UTexture2D> Icon = nullptr;

	// Description
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory Framework|Item Definition")
    FText Description;

    // Functions can be Override and Implement in Child Classes

    UFUNCTION(BlueprintCallable, BlueprintPure)
    virtual UTexture2D* GetIcon() const
    {
        return Icon;
    }

    virtual FText GetDisplayName() const
    {
        return Name;
    }

    virtual void OnItemAcquired(AActor* Buyer) const;

	// Template function to find a fragment of a specific class in the Fragments array

    template <typename T>
    const T* FindFragmentByClass() const
    {
        for (const UItemFragment* Fragment : Fragments)
        {
            if (const T* TypedFragment = Cast<T>(Fragment))
            {
                return TypedFragment;
            }
        }
        return nullptr;
    }
};