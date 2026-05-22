
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidgets.generated.h"

// Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(
	FOnInventoryTransferRequested,
	UInventoryComponent*, SourceInventory,
	UInventoryComponent*, TargetInventory,
	int, SourceIndex,
	int, TargetIndex);


// Forward Declarations
class UInventoryItem;
class UImage;
class UTextBlock;
class UUniformGridPanel;

/**
 *
 */
UCLASS()
class INVENTORYFRAMEWORK_API UInventoryGridWidget : public UUserWidget
{
	GENERATED_BODY()


public:

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> Grid;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory Framework| UI | Items")
	int Columns = 8;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory Framework| UI | Items")
	TSubclassOf<UInventoryItemWidget> ItemWidgetClass = nullptr;

	UFUNCTION(BlueprintCallable, Category = "Inventory Framework| UI | Items")
	void SetInventoryData(UInventoryComponent* NewInventory);

	UPROPERTY(BlueprintReadOnly,
		meta = (ExposeOnSpawn = "true"),
		Category = "Inventory Framework|UI|Items")
	TObjectPtr<UInventoryComponent> InventoryComponent;

	TWeakObjectPtr<UInventoryComponent> Inventory;

	UPROPERTY(BlueprintAssignable)
	FOnInventoryTransferRequested
		OnInventoryTransferRequested;


	bool HandleItemDrop(UDragDropOperation* InOperation, int TargetIndex) const;

protected:

	UFUNCTION(BlueprintCallable, Category = "Inventory Framework| UI | Items")
	void RefreshInventory();

	virtual void NativeDestruct() override;
	virtual void NativeConstruct() override;
};

UCLASS()
class INVENTORYFRAMEWORK_API UInventoryItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY()
	TObjectPtr<UInventoryGridWidget> ParentGridWidget;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> ItemImage;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> StackAmountText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> NameText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory Framework| UI | Items")
	bool bShowNumberAlways = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory Framework| UI | Items")
	TObjectPtr<UTexture2D> EmptySlotIcon;

	UFUNCTION(BlueprintCallable, Category = "Inventory Framework| UI | Items")
	void SetItemData(UInventoryItem* InventoryItem);

	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory Framework| UI | Items")
	void OnSetItemData(UInventoryItem* InventoryItem);

	// Drag and Drop

	virtual FReply NativeOnMouseButtonDown(
		const FGeometry& InGeometry,
		const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnDragDetected(
		const FGeometry& InGeometry,
		const FPointerEvent& InMouseEvent,
		UDragDropOperation*& OutOperation) override;

	virtual bool NativeOnDrop(
		const FGeometry& InGeometry,
		const FDragDropEvent& InDragDropEvent,
		UDragDropOperation* InOperation) override;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory Framework|UI|Items")
	int SlotIndex = INDEX_NONE;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory Framework|UI|Items")
	TWeakObjectPtr<UInventoryComponent> Inventory;


};