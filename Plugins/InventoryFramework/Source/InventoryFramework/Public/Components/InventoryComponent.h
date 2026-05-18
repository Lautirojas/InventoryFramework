
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryChanged);

// Forward declaration
class UInventoryItem;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INVENTORYFRAMEWORK_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Constructor
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

#pragma region Inventory

	// Inventory Array
	UPROPERTY(BlueprintReadOnly, Instanced, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Inventory Framework|Inventory")
	TArray<TObjectPtr<UInventoryItem>> Items; // The array that holds the items in the inventory, using TObjectPtr for safe memory management and garbage collection

	// Inventory Size
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Inventory Framework|Inventory")
	int Size = 10; // The maximum number of items that can be stored in the inventory

	// EVENTS

	UPROPERTY(BlueprintAssignable)
	FOnInventoryChanged OnInventoryChanged; // Event triggered when the inventory changes (item added, removed, etc.)

#pragma endregion Inventory

public:	

#pragma region Helpers

	// Get Size of the Inventory
	UFUNCTION(BlueprintCallable, Category = "Inventory Framework|Inventory")
	int GetSize() const; // const because it doesn't modify the inventory // Get the size of the inventory

	// Get all items in the inventory
	UFUNCTION(BlueprintCallable, Category = "Inventory Framework|Inventory")
	const TArray<UInventoryItem*> GetAllItems() const; // const because it doesn't modify the inventory // Get all items in the inventory

#pragma endregion Helpers

#pragma region InventoryFunctions

	// Get Item by Definition // Data Asset used to define the item
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory Framework|Inventory")
	bool FindItemByDefinition(UInventoryItemDefinition* ItemDefinition, int& Index) const; // returns true if the item was found in the inventory, and sets the index of the item in the inventory

	// Remove Item Amount by Definition // Data Asset used to define the item
	UFUNCTION(BlueprintCallable, Category = "Inventory Framework|Inventory")
	bool RemoveItemAmountByDefinition(UInventoryItemDefinition* ItemDefinition, int Amount = 1); // returns true if the item was removed successfully

	// Get Item Amount by Definition // Data Asset used to define the item
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory Framework|Inventory")
	int GetItemAmountByDefinition(UInventoryItemDefinition* ItemDefinition) const; // returns the amount of items in the inventory that match the given definition

	// Add Item by Definition // Data Asset used to define the item
	UFUNCTION(BlueprintCallable, Category = "Inventory Framework|Inventory")
	bool AddItem(UInventoryItemDefinition* ItemDefinition, int Amount = 1); // returns true if the item was added successfully

	// Remove Item at Index
	UFUNCTION(BlueprintCallable, Category = "Inventory Framework|Inventory")
	bool RemoveItemAt(int ItemIndex); // returns true if the item was removed successfully

#pragma endregion InventoryFunctions
		
};
