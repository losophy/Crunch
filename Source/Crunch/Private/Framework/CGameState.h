// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Player/PlayerInfoTypes.h"
#include "CGameState.generated.h"

class UPA_CharacterDefination;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerSelectionUpdated, const TArray<FPlayerSelection>& /*NewPlayerSelection*/);
/**
 *
 */
UCLASS()
class ACGameState : public AGameStateBase
{
	GENERATED_BODY()
public:
	void RequestPlayerSelectionChange(const APlayerState* RequestingPlayer, uint8 DesiredSlot);
	void SetCharacterSelected(const APlayerState* SelectingPlayer, const UPA_CharacterDefination* SelectedDefination);
	bool IsSlotOccupied(uint8 SlotId) const;
	bool IsDefiniationSelected(const UPA_CharacterDefination* Definiation) const;
	void SetCharacterDeselected(const UPA_CharacterDefination* DefiniationToDeselect);

	FOnPlayerSelectionUpdated OnPlayerSelectionUpdated;
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
	const TArray<FPlayerSelection>& GetPlayerSelection() const;
	bool CanStartHeroSelection() const;
	bool CanStartMatch() const;
private:
	UPROPERTY(ReplicatedUsing = OnRep_PlayerSelectionArray)
	TArray<FPlayerSelection> PlayerSelectionArray;

	UFUNCTION()
	void OnRep_PlayerSelectionArray();
};
