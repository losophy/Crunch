// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GameplayWidget.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Components/WidgetSwitcher.h"
#include "Components/CanvasPanel.h"
#include "GAS/CAbilitySystemComponent.h"
#include "AbilitySystemComponent.h"
#include "Widgets/AbilityListView.h"
#include "Widgets/GameplayMenu.h"
#include "Widgets/ShopWidget.h"
#include "Widgets/ValueGauge.h"
#include "GAS/CAttributeSet.h"


void UGameplayWidget::NativeConstruct()
{
	Super::NativeConstruct();

	OwnerAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwningPlayerPawn());

	if (OwnerAbilitySystemComponent)
	{
		HealthBar->SetAndBoundToGameplayAttribute(OwnerAbilitySystemComponent, UCAttributeSet::GetHealthAttribute(), UCAttributeSet::GetMaxHealthAttribute());
		ManaBar->SetAndBoundToGameplayAttribute(OwnerAbilitySystemComponent, UCAttributeSet::GetManaAttribute(), UCAttributeSet::GetMaxManaAttribute());
	}

	SetShowMouseCursor(false);
	SetFocusToGameOnly();
	if (GameplayMenu)
	{
		GameplayMenu->GetResumeButtonClickedEventDelegate().AddDynamic(this, &UGameplayWidget::ToggleGameplayMenu);
	}
}

void UGameplayWidget::ConfigureAbilities(const TMap<ECAbilityInputID, TSubclassOf<class UGameplayAbility>>& Abilities)
{
	AbilityListView->ConfigureAbilities(Abilities);
}

void UGameplayWidget::ToggleShop()
{
	if (ShopWidget->GetVisibility() == ESlateVisibility::HitTestInvisible)
	{
		ShopWidget->SetVisibility(ESlateVisibility::Visible);
		PlayShopPopupAnimation(true);
		SetOwningPawnInputEnabled(false);
		SetShowMouseCursor(true);
		SetFocusToGameAndUI();
		ShopWidget->SetFocus();
	}
	else
	{
		ShopWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
		PlayShopPopupAnimation(false);
		SetOwningPawnInputEnabled(true);
		SetShowMouseCursor(false);
		SetFocusToGameOnly();
	}
}

void UGameplayWidget::ToggleGameplayMenu()
{
	if (MainSwitcher->GetActiveWidget() == GameplayMenuRootPanel)
	{
		MainSwitcher->SetActiveWidget(GameplayWidgetRootPanel);
		SetOwningPawnInputEnabled(true);
		SetShowMouseCursor(false);
		SetFocusToGameOnly();
	}
	else
	{
		ShowGameplayMenu();
	}
}

void UGameplayWidget::ShowGameplayMenu()
{
	MainSwitcher->SetActiveWidget(GameplayMenuRootPanel);
	SetOwningPawnInputEnabled(false);
	SetShowMouseCursor(true);
	SetFocusToGameAndUI();
}

void UGameplayWidget::SetGameplayMenuTitle(const FString& NewTitle)
{
	GameplayMenu->SetTitleText(NewTitle);
}

void UGameplayWidget::PlayShopPopupAnimation(bool bPlayForward)
{
	if (bPlayForward)
	{
		PlayAnimationForward(ShopPopupAnimation);
	}
	else
	{
		PlayAnimationReverse(ShopPopupAnimation);
	}
}

void UGameplayWidget::SetOwningPawnInputEnabled(bool bPawnInputEnabled)
{
	if (bPawnInputEnabled)
	{
		GetOwningPlayerPawn()->EnableInput(GetOwningPlayer());
	}
	else
	{
		GetOwningPlayerPawn()->DisableInput(GetOwningPlayer());
	}
}

void UGameplayWidget::SetShowMouseCursor(bool bShowMouseCursor)
{
	GetOwningPlayer()->SetShowMouseCursor(bShowMouseCursor);
}

void UGameplayWidget::SetFocusToGameAndUI()
{
	FInputModeGameAndUI GameAndUIInputMode;
	GameAndUIInputMode.SetHideCursorDuringCapture(false);

	GetOwningPlayer()->SetInputMode(GameAndUIInputMode);
}

void UGameplayWidget::SetFocusToGameOnly()
{
	FInputModeGameOnly GameOnlyInputMode;
	GetOwningPlayer()->SetInputMode(GameOnlyInputMode);
}
