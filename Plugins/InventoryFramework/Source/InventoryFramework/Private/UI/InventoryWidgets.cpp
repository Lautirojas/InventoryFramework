


#include "UI/InventoryWidgets.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/UniformGridSlot.h"
#include "Components/UniformGridPanel.h"
#include "Items/Instances/InventoryItem.h"
#include "Items/Definitions/InventoryItemDefinition.h"
#include "Components/InventoryComponent.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "UI/InventoryDragDropOperation.h"
#include "Input/Reply.h"
#include "InputCoreTypes.h"
#include "InventoryFrameworklog.h"


void UInventoryItemWidget::SetItemData(UInventoryItem* InventoryItem)
{
	// Set Default Values
	UTexture2D* Icon = EmptySlotIcon;
	int StackAmount = 0;
	FText ItemText = FText::FromString("");

	// Set values if InventoryItem is valid
	if (InventoryItem)
	{
		Icon = InventoryItem->Definition->Icon;
		StackAmount = InventoryItem->StackAmount;
		ItemText = InventoryItem->Definition->Name;
	}

	// Use values on widgets
	ItemImage->SetBrushFromTexture(Icon);
	if (bShowNumberAlways || StackAmount >= 2)
	{
		StackAmountText->SetText(FText::FromString(FString::FromInt(StackAmount)));
	}
	else if (StackAmount >= 2) {
		StackAmountText->SetText(FText::FromString(FString::FromInt(StackAmount)));
	}
	else
	{
		StackAmountText->SetText(FText::FromString(""));
	}

	if (NameText)
	{
		NameText->SetText(ItemText);
	}

	// Call Blueprint Event
	OnSetItemData(InventoryItem);
}

void UInventoryGridWidget::SetInventoryData(UInventoryComponent* NewInventory)
{
	if (!IsValid(NewInventory))
	{
		return;
	}

	// unbind previous inventory
	if (Inventory.IsValid())
	{
		Inventory.Get()->OnInventoryChanged.RemoveAll(this);
	}

	Inventory = NewInventory;

	// bind inventory delegate
	NewInventory->OnInventoryChanged.AddDynamic(
		this,
		&UInventoryGridWidget::RefreshInventory);

	// initial refresh
	RefreshInventory();
}

void UInventoryGridWidget::RefreshInventory()
{
	if (!Inventory.IsValid())
	{
		return;
	}

	if (!Grid)
	{
		return;
	}

	Grid->ClearChildren();

	const TArray<UInventoryItem*>& Items =
		Inventory->GetAllItems();

	UE_LOG(LogInventoryFramework, Log, TEXT("Refreshing Inventory Grid with %d items"), Items.Num());

	for (int i = 0; i < Items.Num(); ++i)
	{
		UInventoryItemWidget* NewItemWidget =
			CreateWidget<UInventoryItemWidget>(
				GetWorld(),
				ItemWidgetClass);

		if (!ensure(NewItemWidget))
		{
			continue;
		}

		NewItemWidget->ParentGridWidget = this;
		NewItemWidget->Inventory = Inventory;
		NewItemWidget->SlotIndex = i;

		NewItemWidget->SetItemData(Items[i]);

		UUniformGridSlot* GridSlot =
			Grid->AddChildToUniformGrid(NewItemWidget);

		GridSlot->SetColumn(i % Columns);
		GridSlot->SetRow(i / Columns);
		GridSlot->SetHorizontalAlignment(HAlign_Fill);
		GridSlot->SetVerticalAlignment(VAlign_Fill);
	}
}

void UInventoryGridWidget::NativeDestruct()
{
	if (Inventory.IsValid())
	{
		Inventory.Get()->OnInventoryChanged.RemoveAll(this);
	}


	Super::NativeDestruct();
}

void UInventoryGridWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(InventoryComponent))
	{
		SetInventoryData(InventoryComponent);
	}
}

bool UInventoryGridWidget::HandleItemDrop(
	UDragDropOperation* InOperation,
	int TargetIndex) const
{
	const UInventoryDragDropOperation* DragOperation =
		Cast<UInventoryDragDropOperation>(InOperation);

	if (!DragOperation)
	{
		return false;
	}

	if (!Inventory.IsValid() ||
		!DragOperation->SourceInventory.IsValid())
	{
		return false;
	}

	// Broadcast transfer request
	OnInventoryTransferRequested.Broadcast(
		DragOperation->SourceInventory.Get(),
		Inventory.Get(),
		DragOperation->SourceIndex,
		TargetIndex);

	return true;
}

#pragma region ItemDragDropFunctions

		FReply UInventoryItemWidget::NativeOnMouseButtonDown(
			const FGeometry& InGeometry,
			const FPointerEvent& InMouseEvent)
		{
			return UWidgetBlueprintLibrary::DetectDragIfPressed(
				InMouseEvent,
				this,
				EKeys::LeftMouseButton).NativeReply;
		}

		void UInventoryItemWidget::NativeOnDragDetected(
			const FGeometry& InGeometry,
			const FPointerEvent& InMouseEvent,
			UDragDropOperation*& OutOperation)
		{
			UInventoryDragDropOperation* DragOperation =
				NewObject<UInventoryDragDropOperation>();

			DragOperation->SourceInventory = Inventory;
			DragOperation->DefaultDragVisual = this;
			DragOperation->SourceIndex = SlotIndex;

			OutOperation = DragOperation;
		}

		bool UInventoryItemWidget::NativeOnDrop(
			const FGeometry& InGeometry,
			const FDragDropEvent& InDragDropEvent,
			UDragDropOperation* InOperation)
		{
			const UInventoryDragDropOperation* DragOperation =
				Cast<UInventoryDragDropOperation>(InOperation);

			if (!DragOperation)
			{
				return false;
			}

			//if (!Inventory.IsValid() ||
			//	!DragOperation->SourceInventory.IsValid())
			//{
			//	return false;
			//}

			if (!ParentGridWidget)
			{
				return false;
			}

			//DragOperation->SourceInventory->TransferItemTo(
			//	Inventory.Get(),
			//	DragOperation->SourceIndex,
			//	SlotIndex);

			ParentGridWidget->HandleItemDrop(
				InOperation,
				SlotIndex);

			return true;
		}

#pragma endregion ItemDragDropFunctions