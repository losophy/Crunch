// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/InventoryWidget.h"
#include "Inventory/InventoryComponent.h"
#include "Widgets/InventoryItemWidget.h"
#include "Components/WrapBox.h"
#include "Components/WrapBoxSlot.h"

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (APawn* OwnerPawn = GetOwningPlayerPawn())
	{
		InventoryComponent = OwnerPawn->GetComponentByClass<UInventoryComponent>();
		if (InventoryComponent)
		{
			InventoryComponent->OnItemAdded.AddUObject(this, &UInventoryWidget::ItemAdded);
			InventoryComponent->OnItemRemoved.AddUObject(this, &UInventoryWidget::ItemRemoved);
			InventoryComponent->OnItemStackCountChanged.AddUObject(this, &UInventoryWidget::ItemStackCountChanged);
			int Capacity = InventoryComponent->GetCapacity();

			ItemList->ClearChildren();

			for (int i = 0; i < Capacity; ++i)
			{
				UInventoryItemWidget* NewEmptyWidget = CreateWidget<UInventoryItemWidget>(GetOwningPlayer(), ItemWidgetClass);
				if (NewEmptyWidget)
				{
					NewEmptyWidget->SetSlotNumber(i);
					UWrapBoxSlot* NewItemSlot = ItemList->AddChildToWrapBox(NewEmptyWidget);
					NewItemSlot->SetPadding(FMargin(2.f));
					ItemWidgets.Add(NewEmptyWidget);

					NewEmptyWidget->OnInventoryItemDropped.AddUObject(this, &UInventoryWidget::HandleItemDragDrop);
					NewEmptyWidget->OnLeftBttonClicked.AddUObject(InventoryComponent, &UInventoryComponent::TryActivateItem);
				}
			}

		}
	}
}

void UInventoryWidget::ItemAdded(const UInventoryItem* InventoryItem)
{
	if (!InventoryItem)
		return;

	if (UInventoryItemWidget* NextAvaliableSlot = GetNextAvaliableSlot())
	{
		NextAvaliableSlot->UpdateInventoryItem(InventoryItem);
		PopulatedItemEntryWidgets.Add(InventoryItem->GetHandle(), NextAvaliableSlot);
		if (InventoryComponent)
		{
			InventoryComponent->ItemSlotChanged(InventoryItem->GetHandle(), NextAvaliableSlot->GetSlotNumber());
		}
	}
}

void UInventoryWidget::ItemStackCountChanged(const FInventoryItemHandle& Handle, int NewCount)
{
	UInventoryItemWidget** FoundWidget = PopulatedItemEntryWidgets.Find(Handle);
	if (FoundWidget)
	{
		(*FoundWidget)->UpdateStackCount();
	}
}

UInventoryItemWidget* UInventoryWidget::GetNextAvaliableSlot() const
{
	for (UInventoryItemWidget* Widget : ItemWidgets)
	{
		if (Widget->IsEmpty())
		{
			return Widget;
		}
	}

	return nullptr;
}

void UInventoryWidget::HandleItemDragDrop(UInventoryItemWidget* DestinationWidget, UInventoryItemWidget* SourceWidget)
{
	const UInventoryItem* SrcItem = SourceWidget->GetInventoryItem();
	const UInventoryItem* DestinationItem = DestinationWidget->GetInventoryItem();

	DestinationWidget->UpdateInventoryItem(SrcItem);
	SourceWidget->UpdateInventoryItem(DestinationItem);

	PopulatedItemEntryWidgets[DestinationWidget->GetItemHandle()] = DestinationWidget;

	if (InventoryComponent)
	{
		InventoryComponent->ItemSlotChanged(DestinationWidget->GetItemHandle(), DestinationWidget->GetSlotNumber());
	}

	if (!SourceWidget->IsEmpty())
	{
		PopulatedItemEntryWidgets[SourceWidget->GetItemHandle()] = SourceWidget;
		if (InventoryComponent)
		{
			InventoryComponent->ItemSlotChanged(SourceWidget->GetItemHandle(), SourceWidget->GetSlotNumber());
		}
	}
}

void UInventoryWidget::ItemRemoved(const FInventoryItemHandle& ItemHandle)
{
	UInventoryItemWidget** FoundWidget = PopulatedItemEntryWidgets.Find(ItemHandle);
	if (FoundWidget && *FoundWidget)
	{
		(*FoundWidget)->EmptySlot();
		PopulatedItemEntryWidgets.Remove(ItemHandle);
	}
}
