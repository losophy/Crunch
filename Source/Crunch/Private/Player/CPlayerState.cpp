// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CPlayerState.h"
#include "Character/PA_CharacterDefination.h"
#include "Character/CCharacter.h"
#include "Net/UnrealNetwork.h"
#include "Network/CNetStatics.h"
#include "Framework/CGameState.h"
#include "Kismet/GameplayStatics.h"

ACPlayerState::ACPlayerState()
{
	bReplicates = true;
	NetUpdateFrequency = 100.f;
}

void ACPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACPlayerState, PlayerSelection);
}

void ACPlayerState::BeginPlay()
{
	Super::BeginPlay();
	CGameState = Cast<ACGameState>(UGameplayStatics::GetGameState(this));

	if (CGameState)
	{
		CGameState->OnPlayerSelectionUpdated.AddUObject(this, &ACPlayerState::PlayerSelectionUpdated);
	}
}

void ACPlayerState::CopyProperties(APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);

	ACPlayerState* NewPlayerState = Cast < ACPlayerState>(PlayerState);
	if (NewPlayerState)
	{
		NewPlayerState->PlayerSelection = PlayerSelection;
	}
}

TSubclassOf<APawn> ACPlayerState::GetSelectedPawnClass() const
{
	if (PlayerSelection.GetCharacterDefination())
	{
		return PlayerSelection.GetCharacterDefination()->LoadCharacterClass();
	}

	return nullptr;
}

FGenericTeamId ACPlayerState::GetTeamIdBasedOnSlot() const
{
	return PlayerSelection.GetPlayerSlot() < UCNetStatics::GetPlayerCountPerTeam() ? FGenericTeamId{ 0 } : FGenericTeamId{ 1 };
}

void ACPlayerState::Server_SetSelectedCharacterDefination_Implementation(const UPA_CharacterDefination* NewDefination)
{
	if (!CGameState)
		return;

	if (!NewDefination)
		return;

	if (CGameState->IsDefiniationSelected(NewDefination))
		return;

	if (PlayerSelection.GetCharacterDefination())
	{
		CGameState->SetCharacterDeselected(PlayerSelection.GetCharacterDefination());
	}

	PlayerSelection.SetCharacterDefination(NewDefination);
	CGameState->SetCharacterSelected(this, NewDefination);
}

bool ACPlayerState::Server_SetSelectedCharacterDefination_Validate(const UPA_CharacterDefination* NewDefination)
{
	return true;
}

void ACPlayerState::PlayerSelectionUpdated(const TArray<FPlayerSelection>& NewPlayerSelections)
{
	for (const FPlayerSelection& NewPlayerSelection : NewPlayerSelections)
	{
		if (NewPlayerSelection.IsForPlayer(this))
		{
			PlayerSelection = NewPlayerSelection;
		}
	}
}
