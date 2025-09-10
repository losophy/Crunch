// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GameplayMenu.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/TextBlock.h"

void UGameplayMenu::NativeConstruct()
{
	Super::NativeConstruct();
	MainMenuBtn->OnClicked.AddDynamic(this, &UGameplayMenu::BackToMainMenu);
	QuitGameBtn->OnClicked.AddDynamic(this, &UGameplayMenu::QuitGame);

}

FOnButtonClickedEvent& UGameplayMenu::GetResumeButtonClickedEventDelegate()
{
	return ResumeBtn->OnClicked;
}

void UGameplayMenu::SetTitleText(const FString& NewTitle)
{
	MenuTitle->SetText(FText::FromString(NewTitle));
}

void UGameplayMenu::BackToMainMenu()
{

}

void UGameplayMenu::QuitGame()
{
	UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, true);
}
