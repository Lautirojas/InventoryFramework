


#include "UI/InventoryWidgets.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/UniformGridSlot.h"
#include "Components/UniformGridPanel.h"
#include "Items/Instances/InventoryItem.h"
#include "Items/Definitions/InventoryItemDefinition.h"
#include "Components/InventoryComponent.h"

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
	// Clear existing items
	Grid->ClearChildren();

	Inventory = NewInventory;

	const TArray<UInventoryItem*>& Items = NewInventory->GetAllItems();

	// For Each item in inventory, create a new InventoryItemWidget and add to grid
	for (int i = 0; i < NewInventory->GetSize(); ++i)
	{
		UInventoryItemWidget* NewItemWidget = CreateWidget<UInventoryItemWidget>(GetWorld(), ItemWidgetClass);

		if (i < Items.Num())
		{
			NewItemWidget->SetItemData(Items[i]);
		}
		else
		{
			NewItemWidget->SetItemData(nullptr);
		}

		UUniformGridSlot* GridSlot = Grid->AddChildToUniformGrid(NewItemWidget);
		GridSlot->SetColumn(i % Columns);
		GridSlot->SetRow(i / Columns);
		GridSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
		GridSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
	}
}