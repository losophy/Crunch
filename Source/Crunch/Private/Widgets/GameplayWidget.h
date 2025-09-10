// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GAS/CGameplayAbilityTypes.h"
#include "GameplayWidget.generated.h"

/**
 * 
 */
UCLASS()
class UGameplayWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	void ConfigureAbilities(const TMap<ECAbilityInputID, TSubclassOf<class UGameplayAbility>>& Abilities);
	void ToggleShop();

	UFUNCTION()
	void ToggleGameplayMenu();

	void ShowGameplayMenu();
	void SetGameplayMenuTitle(const FString& NewTitle);

private:
	UPROPERTY(meta = (BindWidget))
	class UValueGauge* HealthBar;

	UPROPERTY(meta = (BindWidget))
	class UValueGauge* ManaBar;

	UPROPERTY(meta = (BindWidget))
	class UAbilityListView* AbilityListView;

	UPROPERTY(meta = (BindWidget))
	class UStatsGauge* AttackDamageGauge;

	UPROPERTY(meta = (BindWidget))
	class UStatsGauge* ArmorGauge;

	UPROPERTY(meta = (BindWidget))
	class UStatsGauge* MoveSpeedGauge;

	UPROPERTY(meta = (BindWidget))
	class UStatsGauge* IntenlligenceGauge;

	UPROPERTY(meta = (BindWidget))
	class UStatsGauge* StrengthGauge;

	UPROPERTY(meta = (BindWidget))
	class UShopWidget* ShopWidget;

	UPROPERTY(meta = (BindWidget))
	class UInventoryWidget* InventoryWidget;

	UPROPERTY(meta = (BindWidget))
	class USkeletalMeshRenderWidget* HeadshotWidget;

	UPROPERTY(meta = (BindWidget))
	class UMatchStatWidget* MatchStatWidget;

	UPROPERTY(meta = (BindWidget))
	class UGameplayMenu* GameplayMenu;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* MainSwitcher;

	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* GameplayWidgetRootPanel;

	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* GameplayMenuRootPanel;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* ShopPopupAnimation;

	void PlayShopPopupAnimation(bool bPlayForward);
	void SetOwningPawnInputEnabled(bool bPawnInputEnabled);
	void SetShowMouseCursor(bool bShowMouseCursor);
	void SetFocusToGameAndUI();
	void SetFocusToGameOnly();

	UPROPERTY()
	class UAbilitySystemComponent* OwnerAbilitySystemComponent;
};
