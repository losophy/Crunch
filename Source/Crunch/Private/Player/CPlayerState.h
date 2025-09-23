// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Player/PlayerInfoTypes.h"
#include "CPlayerState.generated.h"

class UPA_CharacterDefination;
/**
 * 
 */
UCLASS()
class ACPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	ACPlayerState();
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
	virtual void BeginPlay() override;
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetSelectedCharacterDefination(const UPA_CharacterDefination* NewDefination);

private:
	UPROPERTY(Replicated)
	FPlayerSelection PlayerSelection;

	UPROPERTY()
	class ACGameState* CGameState;

	void PlayerSelectionUpdated(const TArray<FPlayerSelection>& NewPlayerSelections);
};
