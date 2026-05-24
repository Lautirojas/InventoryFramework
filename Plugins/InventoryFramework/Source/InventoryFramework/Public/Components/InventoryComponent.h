
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Net/UnrealNetwork.h"
#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryChanged);

// Forward declaration
class UInventoryItem;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INVENTORYFRAMEWORK_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()


protected:

	// Called when the game starts
	virtual void BeginPlay() override;

public:

	// Constructor
	UInventoryComponent();

	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

#pragma region InventoryProperties

	// Inventory Size
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory Framework|Inventory")
	int Size = 10;

	#pragma region Multiplayer

		UPROPERTY(ReplicatedUsing = OnRep_Items, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Instanced, Category = "Inventory Framework|Inventory")
		TArray<TObjectPtr<UInventoryItem>> Items;

	#pragma endregion Multiplayer

#pragma endregion InventoryProperties

public:

#pragma region InventoryQueries

	// Returns the total slot capacity of the inventory.
	UFUNCTION(BlueprintCallable, Category = "Inventory Framework|Inventory")
	int GetSize() const;

	// Get all items in inventory
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory Framework|Inventory")
	const TArray<UInventoryItem*>& GetAllItems() const;

	// Check if inventory is full
	UFUNCTION(BlueprintCallable, Category = "Inventory Framework|Inventory")
	bool IsInventoryFull() const;

	// Check if inventory has at least one free slot
	UFUNCTION(BlueprintCallable, Category = "Inventory Framework|Inventory")
	bool HasFreeSlot() const;

	// Get total free slot count
	UFUNCTION(BlueprintCallable, Category = "Inventory Framework|Inventory")
	int GetFreeSlotCount() const;

	// Check if inventory contains item definition
	UFUNCTION(BlueprintCallable, Category = "Inventory Framework|Inventory")
	bool ContainsItemDefinition(UInventoryItemDefinition* ItemDefinition) const;

	// Check if inventory can fit item amount
	UFUNCTION(BlueprintCallable, Category = "Inventory Framework|Inventory")
	bool HasSpaceForItem(UInventoryItemDefinition* ItemDefinition, int Amount) const;

	// Find item index by definition
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory Framework|Inventory")
	bool FindItemIndexByDefinition(UInventoryItemDefinition* ItemDefinition, int& Index) const;

	// Get total amount of item definition
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory Framework|Inventory")
	int GetItemAmountByDefinition(UInventoryItemDefinition* ItemDefinition) const;

#pragma endregion InventoryQueries

#pragma region InventoryMutation

	// Add item-s to inventory
	UFUNCTION(BlueprintCallable, Category = "Inventory Framework|Inventory")
	bool AddItem(UInventoryItemDefinition* ItemDefinition, int Amount = 1);

	// Remove item amount by definition
	UFUNCTION(BlueprintCallable, Category = "Inventory Framework|Inventory")
	bool RemoveItemAmountByDefinition(UInventoryItemDefinition* ItemDefinition, int Amount = 1);

	// Remove item at slot index
	UFUNCTION(BlueprintCallable, Category = "Inventory Framework|Inventory")
	bool RemoveItemAt(int ItemIndex);

#pragma endregion InventoryMutation

#pragma region InventoryTransfer

	bool MoveItem(int FromIndex, int ToIndex);

	UFUNCTION(BlueprintCallable, Category = "Inventory Framework|Inventory|DragDrop")
	bool TransferItemTo(
		UInventoryComponent* TargetInventory,
		int SourceIndex,
		int TargetIndex);

#pragma endregion InventoryTransfer

#pragma region Multiplayer

	// OnRep for items array
	UFUNCTION()
	void OnRep_Items();

#pragma endregion Multiplayer

#pragma region Events

	// Event triggered when the inventory changes
	UPROPERTY(BlueprintAssignable, Category = "Inventory Framework|Events")
	FOnInventoryChanged OnInventoryChanged;

#pragma endregion Events

private:

#pragma region InternalHelpers

	// Create inventory item 
	UInventoryItem* CreateInventoryItem(UInventoryItemDefinition* ItemDefinition, int StackAmount);

	// Find first empty inventory slot
	int FindFirstEmptySlot() const;

	// Broadcast inventory changed event
	void NotifyInventoryChanged();

	// Drag and drop helper functions

	// Check if slot index is valid
	bool IsValidSlotIndex(int Index) const;

#pragma endregion InternalHelpers

};
