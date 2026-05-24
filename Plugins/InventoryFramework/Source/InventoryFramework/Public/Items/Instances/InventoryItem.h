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

	UPROPERTY(Replicated, VisibleAnywhere, Category = "Inventory Framework|Inventory Item")
	int32 StackAmount = 0; // The amount of items in the stack 

	// Helpers

	UFUNCTION(BlueprintCallable, Category = "Inventory Framework|Inventory")
	void Initialize(
		UInventoryItemDefinition* InDefinition,
		int32 InitialStackAmount);

	UFUNCTION(BlueprintCallable, Category = "Inventory Framework|Inventory Item")
	void AddToStack(int32 Amount);

	UFUNCTION(BlueprintCallable, Category = "Inventory Framework|Inventory Item")
	void RemoveFromStack(int32 Amount);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory Framework|Inventory Item")
	bool IsStackFull() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory Framework|Inventory Item")
	const UInventoryItemDefinition* GetInventoryDefinition() const;

	bool HasDefinition(
		const UInventoryItemDefinition* ItemDefinition) const;

	UFUNCTION(BlueprintPure, Category = "Inventory Framework|Inventory Item")
	bool CanMergeWithDefinition(
		const UInventoryItemDefinition* ItemDefinition) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory Framework|Inventory Item")
	bool CanMergeWith(const UInventoryItem* OtherItem) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory Framework|Inventory Item")
	int32 GetRemainingStackSpace() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory Framework|Inventory Item")
	int32 GetMaxStackSize() const;

	UFUNCTION(BlueprintPure, Category = "Inventory Framework|Inventory Item")
	bool IsEmpty() const;

	int32 MergeFrom(UInventoryItem* OtherItem);
};
