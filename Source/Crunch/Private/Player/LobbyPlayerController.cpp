// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/LobbyPlayerController.h"
#include "GameFramework/PlayerState.h"
#include "Framework/CGameInstance.h"
#include "Framework/CGameState.h"

void ALobbyPlayerController::Server_RequestSlotSelectionChange_Implementation(uint8 NewSlotID)
{
	if (!GetWorld())
		return;

	ACGameState* CGameState = GetWorld()->GetGameState<ACGameState>();
	if (!CGameState)
		return;

	CGameState->RequestPlayerSelectionChange(GetPlayerState<APlayerState>(), NewSlotID);
}

bool ALobbyPlayerController::Server_RequestSlotSelectionChange_Validate(uint8 NewSlotID)
{
	return true;
}

void ALobbyPlayerController::Server_StartHeroSelection_Implementation()
{
	if (!HasAuthority() || !GetWorld())
		return;

	for (FConstPlayerControllerIterator PlayerControllerIterator = GetWorld()->GetPlayerControllerIterator(); PlayerControllerIterator; ++PlayerControllerIterator)
	{
		ALobbyPlayerController* PlayerController = Cast<ALobbyPlayerController>(*PlayerControllerIterator);
		if (PlayerController)
		{
			PlayerController->Client_StartHeroSelection();
		}
	}
}

bool ALobbyPlayerController::Server_StartHeroSelection_Validate()
{
	return true;
}

void ALobbyPlayerController::Client_StartHeroSelection_Implementation()
{
	OnSwitchToHeroSelection.ExecuteIfBound();
}

ALobbyPlayerController::ALobbyPlayerController()
{
	bAutoManageActiveCameraTarget = false;
}

void ALobbyPlayerController::Server_RequestStartMatch_Implementation()
{
	UCGameInstance* CGameInstance = GetGameInstance<UCGameInstance>();
	if (CGameInstance)
	{
		CGameInstance->StartMatch();
	}
}

bool ALobbyPlayerController::Server_RequestStartMatch_Validate()
{
	return true;
}
