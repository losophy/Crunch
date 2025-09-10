// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "GameplayMenu.generated.h"

/**
 * 
 */
UCLASS()
class UGameplayMenu : public UUserWidget
{
	GENERATED_BODY()
	virtual void NativeConstruct() override;
	FOnButtonClickedEvent& GetResumeButtonClickedEventDelegate();

private:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MenuTitle;

	UPROPERTY(meta = (BindWidget))
	class UButton* ResumeBtn;

	UPROPERTY(meta = (BindWidget))
	class UButton* MainMenuBtn;

	UPROPERTY(meta = (BindWidget))
	class UButton* QuitGameBtn;

	UFUNCTION()
	void BackToMainMenu();

	UFUNCTION()
	void QuitGame();
};
