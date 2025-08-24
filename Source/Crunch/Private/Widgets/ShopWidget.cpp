// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/ShopWidget.h"
#include "Framework/CAssetManager.h"
#include "Widgets/ShopItemWidget.h"
#include "Inventory/InventoryComponent.h"
#include "Components/TileView.h"

void UShopWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SetIsFocusable(true);
	LoadShopItems();
	ShopItemList->OnEntryWidgetGenerated().AddUObject(this, &UShopWidget::ShopItemWidgetGenerated);
	if (APawn* OwnerPawn = GetOwningPlayerPawn())
	{
		OwnerInventoryComponent = OwnerPawn->GetComponentByClass<UInventoryComponent>();
	}
}

void UShopWidget::LoadShopItems()
{
	UCAssetManager::Get().LoadShopItems(FStreamableDelegate::CreateUObject(this, &UShopWidget::ShopItemLoadFinished));
}

void UShopWidget::ShopItemLoadFinished()
{
	TArray<const UPA_ShopItem*> ShopItems;
	UCAssetManager::Get().GetLoadedShopItems(ShopItems);
	for (const UPA_ShopItem* ShopItem : ShopItems)
	{
		ShopItemList->AddItem(const_cast<UPA_ShopItem*>(ShopItem));
	}
}

void UShopWidget::ShopItemWidgetGenerated(UUserWidget& NewWidget)
{
	UShopItemWidget* ItemWidget = Cast<UShopItemWidget>(&NewWidget);
	if (ItemWidget)
	{
		if (OwnerInventoryComponent)
		{
			ItemWidget->OnItemPurchaseIssued.AddUObject(OwnerInventoryComponent, &UInventoryComponent::TryPurchase);
		}
		ItemsMap.Add(ItemWidget->GetShopItem(), ItemWidget);
	}
}
