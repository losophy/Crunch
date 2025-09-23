// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "CharacterEntryWidget.generated.h"

class UPA_CharacterDefination;
/**
 * 
 */
UCLASS()
class UCharacterEntryWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
public:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	FORCEINLINE const UPA_CharacterDefination* GetCharacterDefination() const { return CharacterDefination; }
	void SetSelected(bool bIsSelected);
private:
	UPROPERTY(meta = (BindWidget))
	class UImage* CharacterIcon;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CharacterNameText;

	UPROPERTY(EditDefaultsOnly, Category = "Character")
	FName IconTextureMatParamName = "Icon";

	UPROPERTY(EditDefaultsOnly, Category = "Character")
	FName SaturationMatParamName = "Saturation";

	UPROPERTY()
	const UPA_CharacterDefination* CharacterDefination;
};
