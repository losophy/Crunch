// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/InventoryItemWidget.h"
#include "Inventory/InventoryItem.h"
#include "Components/TextBlock.h"
#include "Inventory/PA_ShopItem.h"
#include "Widgets/ItemToolTip.h"

void UInventoryItemWidget::NativeConstruct()
{
	Super::NativeConstruct();
	EmptySlot();
}

bool UInventoryItemWidget::IsEmpty() const
{
	return !InventoryItem || !(InventoryItem->IsValid());
}

void UInventoryItemWidget::SetSlotNumber(int NewSlotNumber)
{
	SlotNumber = NewSlotNumber;
}

void UInventoryItemWidget::UpdateInventoryItem(const UInventoryItem* Item)
{
	InventoryItem = Item;
	if (!InventoryItem || !InventoryItem->IsValid() || InventoryItem->GetStackCount() <= 0)
	{
		EmptySlot();
		return;
	}

	SetIcon(Item->GetShopItem()->GetIcon());
	UItemToolTip* ToolTip = SetToolTipWidget(InventoryItem->GetShopItem());
	if (ToolTip)
	{
		ToolTip->SetPrice(InventoryItem->GetShopItem()->GetSellPrice());
	}

	if (InventoryItem->GetShopItem()->GetIsStackable())
	{
		StackCountText->SetVisibility(ESlateVisibility::Visible);
		UpdateStackCount();
	}
	else
	{
		StackCountText->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UInventoryItemWidget::EmptySlot()
{
	InventoryItem = nullptr;
	SetIcon(EmptyTexture);
	SetToolTip(nullptr);

	StackCountText->SetVisibility(ESlateVisibility::Hidden);
	ManaCostText->SetVisibility(ESlateVisibility::Hidden);
	CooldownCountText->SetVisibility(ESlateVisibility::Hidden);
	CooldownDurationText->SetVisibility(ESlateVisibility::Hidden);
}

void UInventoryItemWidget::UpdateStackCount()
{
	if (InventoryItem)
	{
		StackCountText->SetText(FText::AsNumber(InventoryItem->GetStackCount()));
	}
}

UTexture2D* UInventoryItemWidget::GetIconTexture() const
{
	if (InventoryItem && InventoryItem->GetShopItem())
	{
		return InventoryItem->GetShopItem()->GetIcon();
	}

	return nullptr;
}
