// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "InventoryDragDropOperation.generated.h"

// Forward Declarations
class UInventoryComponent;
class UInventoryItem;

/**
 * 
 */
UCLASS()
class INVENTORYFRAMEWORK_API UInventoryDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<UInventoryComponent> SourceInventory;

	UPROPERTY(BlueprintReadOnly)
	int SourceIndex = INDEX_NONE;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UInventoryItem> DraggedItem;
};
