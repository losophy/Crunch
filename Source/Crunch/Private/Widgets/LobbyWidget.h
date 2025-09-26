// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Player/PlayerInfoTypes.h"
#include "LobbyWidget.generated.h"

/**
 * 
 */
UCLASS()
class ULobbyWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
private:
	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* MainSwitcher;

	UPROPERTY(meta = (BindWidget))
	class UWidget* TeamSelectionRoot;

	UPROPERTY(meta = (BindWidget))
	class UButton* StartHeroSelectionButton;

	UPROPERTY(meta = (BindWidget))
	class UUniformGridPanel* TeamSelectionSlotGridPanel;

	UPROPERTY(EditDefaultsOnly, Category = "TeamSelection")
	TSubclassOf<class UTeamSelectionWidget> TeamSelectionWidgetClass;

	UPROPERTY()
	TArray<class UTeamSelectionWidget*> TeamSelectionSlots;

	void ClearAndPopulateTeamSelectionSlots();
	void SlotSelected(uint8 NewSlotID);

	UPROPERTY(meta = (BindWidget))
	class UWidget* HeroSelectionRoot;

	UPROPERTY(meta = (BindWidget))
	class UTileView* CharacterSelectionTileView;

	UPROPERTY(meta = (BindWidget))
	class UAbilityListView* AbilityListView;

	UPROPERTY(meta = (BindWidget))
	class UPlayerTeamLayoutWidget* PlayerTeamLayoutWidget;

	UPROPERTY(meta = (BindWidget))
	class UButton* StartMatchButton;

	UPROPERTY()
	class ALobbyPlayerController* LobbyPlayerController;

	UPROPERTY()
	class ACPlayerState* CPlayerState;

	void ConfigureGameState();
	FTimerHandle ConfigureGameStateTimerHandle;

	UPROPERTY()
	class ACGameState* CGameState;


	void UpdatePlayerSelectionDisplay(const TArray<FPlayerSelection>& PlayerSelections);

	UFUNCTION()
	void StartHeroSelectionButtonClicked();

	void SwitchToHeroSelection();
	void CharacterDefinationLoaded();

	void CharacterSelected(UObject* SelectedUObject);

	UPROPERTY(EditDefaultsOnly, Category = "Character Display")
	TSubclassOf<class ACharacterDisplay> CharacterDisplayClass;

	UPROPERTY()
	class ACharacterDisplay* CharacterDisplay;

	void SpawnCharacterDisplay();
	void UpdateCharacterDisplay(const FPlayerSelection& PlayerSelection);

	UFUNCTION()
	void StartMatchButtonClicked();
};
