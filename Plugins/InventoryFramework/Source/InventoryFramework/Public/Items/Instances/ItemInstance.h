#pragma once

#include "CoreMinimal.h"
#include "ItemInstance.generated.h"

class UItemDefinition;

UCLASS(BlueprintType, EditInlineNew)
class INVENTORYFRAMEWORK_API UItemInstance : public UObject
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory Framework|Instance")
	TObjectPtr<UItemDefinition> Definition = nullptr; // The definition of the item, using TObjectPtr for safe memory management and garbage collection

    template<typename T>
	const T* GetDefinition() const // Template function to get the item definition as a specific class
    {
        return Cast<T>(Definition);
    }
};
