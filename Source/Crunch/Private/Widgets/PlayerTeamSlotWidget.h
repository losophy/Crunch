// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerTeamSlotWidget.generated.h"

class UPA_CharacterDefination;
/**
 *
 */
UCLASS()
class UPlayerTeamSlotWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	void UpdateSlot(const FString& PlayerName, const UPA_CharacterDefination* CharacterDefination);

	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

private:
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* HoverAnim;

	UPROPERTY(meta = (BindWidget))
	class UImage* PlayerCharacterIcon;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* NameText;

	UPROPERTY(EditDefaultsOnly, Category = "Visual")
	FName CharacterIconMatParamName = "Icon";

	UPROPERTY(EditDefaultsOnly, Category = "Visual")
	FName CharacterEmptyMatParamName = "Empty";

	FString CachedPlayerNameStr;
	FString CachedCharacterNameStr;

	void UpdateNameText();
};
