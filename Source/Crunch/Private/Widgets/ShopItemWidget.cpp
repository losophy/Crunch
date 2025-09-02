// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/ShopItemWidget.h"
#include "Components/ListView.h"
#include "Framework/CAssetManager.h"
#include "Inventory/PA_ShopItem.h"

UUserWidget* UShopItemWidget::GetWidget() const
{
	UShopItemWidget* Copy = CreateWidget<UShopItemWidget>(GetOwningPlayer(), GetClass());
	Copy->CopyFromOther(this);
	return Copy;
}

TArray<const ITreeNodeInterface*> UShopItemWidget::GetInputs() const
{
	const FItemCollection* Collection = UCAssetManager::Get().GetCombinationForItem(GetShopItem());
	if (Collection)
		return ItemsToInterfaces(Collection->GetItems());

	return TArray<const ITreeNodeInterface*>{};
}

TArray<const ITreeNodeInterface*> UShopItemWidget::GetOuputs() const
{
	const FItemCollection* Collection = UCAssetManager::Get().GetIngredientForItem(GetShopItem());
	if (Collection)
		return ItemsToInterfaces(Collection->GetItems());

	return TArray<const ITreeNodeInterface*>{};
}

const UObject* UShopItemWidget::GetItemObject() const
{
	return ShopItem;
}

void UShopItemWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
	InitWithShopItem(Cast<UPA_ShopItem>(ListItemObject));
	ParentListView = Cast<UListView>(IUserListEntry::GetOwningListView());
}

void UShopItemWidget::CopyFromOther(const UShopItemWidget* OtherWidget)
{
	OnItemPurchaseIssued = OtherWidget->OnItemPurchaseIssued;
	OnShopItemClicked = OtherWidget->OnShopItemClicked;
	ParentListView = OtherWidget->ParentListView;
	InitWithShopItem(OtherWidget->GetShopItem());
}

void UShopItemWidget::InitWithShopItem(const UPA_ShopItem* NewShopItem)
{
	ShopItem = NewShopItem;
	if (!ShopItem)
	{
		return;
	}

	SetIcon(ShopItem->GetIcon());
	SetToolTipWidget(ShopItem);
}

TArray<const ITreeNodeInterface*> UShopItemWidget::ItemsToInterfaces(const TArray<const UPA_ShopItem*>& Items) const
{
	TArray<const ITreeNodeInterface*> RetInterfaces;
	if (!ParentListView)
		return RetInterfaces;

	for (const UPA_ShopItem* Item : Items)
	{
		const UShopItemWidget* ItemWidget = ParentListView->GetEntryWidgetFromItem<UShopItemWidget>(Item);
		if (ItemWidget)
		{
			RetInterfaces.Add(ItemWidget);
		}
	}

	return RetInterfaces;
}

void UShopItemWidget::RightButtonClicked()
{
	OnItemPurchaseIssued.Broadcast(GetShopItem());
}

void UShopItemWidget::LeftButtonClicked()
{
	OnShopItemClicked.Broadcast(this);
}
