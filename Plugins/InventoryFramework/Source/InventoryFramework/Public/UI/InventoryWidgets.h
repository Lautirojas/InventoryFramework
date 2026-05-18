
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidgets.generated.h"

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

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Inventory Framework| UI | Items")
	TWeakObjectPtr<UInventoryComponent> Inventory;
};

UCLASS()
class INVENTORYFRAMEWORK_API UInventoryItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:
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
};