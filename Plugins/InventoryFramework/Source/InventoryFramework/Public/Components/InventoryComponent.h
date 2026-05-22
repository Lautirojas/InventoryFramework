
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

public:

	// Constructor
	UInventoryComponent();

	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:

	// Called when the game starts
	virtual void BeginPlay() override;

#pragma region InventoryProperties

	// Inventory Size
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory Framework|Inventory")
	int Size = 10;

#pragma endregion InventoryProperties

#pragma region InternalMultiplayerInventory

	UPROPERTY(ReplicatedUsing = OnRep_Items, BlueprintReadOnly,meta = (AllowPrivateAccess = "true"), Instanced, Category = "Inventory Framework|Inventory")
	TArray<TObjectPtr<UInventoryItem>> Items;

#pragma endregion InternalMultiplayerInventory

public:

#pragma region InventoryQueries

	// Get inventory size
	UFUNCTION(BlueprintCallable, Category = "Inventory Framework|Inventory")
	int GetSize() const;

	// Get all items in inventory
	UFUNCTION(BlueprintCallable, Category = "Inventory Framework|Inventory")
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
	bool ContainsItem(UInventoryItemDefinition* ItemDefinition) const;

	// Check if inventory can fit item amount
	UFUNCTION(BlueprintCallable, Category = "Inventory Framework|Inventory")
	bool HasSpaceForItem(UInventoryItemDefinition* ItemDefinition, int Amount) const;

#pragma endregion InventoryQueries

#pragma region InventoryFunctions

	// Find item by definition
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory Framework|Inventory")
	bool FindItemByDefinition(UInventoryItemDefinition* ItemDefinition, int& Index) const;

	// Add item-s to inventory
	UFUNCTION(BlueprintCallable, Category = "Inventory Framework|Inventory")
	bool AddItem(UInventoryItemDefinition* ItemDefinition, int Amount = 1);

	// Remove item amount by definition
	UFUNCTION(BlueprintCallable, Category = "Inventory Framework|Inventory")
	bool RemoveItemAmountByDefinition(UInventoryItemDefinition* ItemDefinition, int Amount = 1);

	// Get total amount of item definition
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory Framework|Inventory")
	int GetItemAmountByDefinition(UInventoryItemDefinition* ItemDefinition) const;

	// Remove item at slot index
	UFUNCTION(BlueprintCallable, Category = "Inventory Framework|Inventory")
	bool RemoveItemAt(int ItemIndex);

#pragma endregion InventoryFunctions

#pragma region InventoryDragDropFunctions

	bool MoveItem(int FromIndex, int ToIndex);

	UFUNCTION(BlueprintCallable, Category = "Inventory Framework|Inventory|DragDrop")
	bool TransferItemTo(
		UInventoryComponent* TargetInventory,
		int SourceIndex,
		int TargetIndex);

#pragma endregion InventoryDragDropFunctions

#pragma region Multiplayer

	// OnRep for items array
	UFUNCTION()
	void OnRep_Items();

#pragma endregion Multiplayer

#pragma region UI

	// Event triggered when the inventory changes
	UPROPERTY(BlueprintAssignable, Category = "Inventory Framework|Events")
	FOnInventoryChanged OnInventoryChanged;

#pragma endregion UI

private:

#pragma region InternalHelpers

	// Create inventory item 
	UInventoryItem* CreateInventoryItem(UInventoryItemDefinition* ItemDefinition, int StackAmount);

	// Find first empty inventory slot
	int FindFirstEmptySlot() const;

	// Check if item can stack
	bool CanStackItem(const UInventoryItem* Item, const UInventoryItemDefinition* Definition) const;

	// Get remaining stack space
	int GetRemainingStackSpace(const UInventoryItem* Item) const;

	// Broadcast inventory changed event
	void NotifyInventoryChanged() const;

	// Drag and drop helper functions

	// Check if slot index is valid
	bool IsValidSlotIndex(int Index) const;

	// Check if item can be stacked with another item
	bool CanMergeStacks(
		UInventoryItem* SourceItem,
		UInventoryItem* TargetItem) const;

#pragma endregion InternalHelpers

};
