// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/CGameMode.h"
#include "GameFramework/PLayerStart.h"
#include "EngineUtils.h"

APlayerController* ACGameMode::SpawnPlayerController(ENetRole InRemoteRole, const FString& Options)
{
	APlayerController* NewPlayerController = Super::SpawnPlayerController(InRemoteRole, Options);
	IGenericTeamAgentInterface* NewPlayerTeamInterface = Cast<IGenericTeamAgentInterface>(NewPlayerController);
	FGenericTeamId TeamId = GetTeamIDForPlayer(NewPlayerController);
	if (NewPlayerTeamInterface)
	{
		NewPlayerTeamInterface->SetGenericTeamId(TeamId);
	}	

	NewPlayerController->StartSpot = FindNextStartSpotForTeam(TeamId);

	return NewPlayerController;
}

FGenericTeamId ACGameMode::GetTeamIDForPlayer(const APlayerController* PlayerController) const
{
	static int PlayerCount = 0;
	++PlayerCount;
	return FGenericTeamId(PlayerCount % 2);
}

AActor* ACGameMode::FindNextStartSpotForTeam(const FGenericTeamId& TeamID) const
{
	const FName* StartSpotTag = TeamStartSpotTagMap.Find(TeamID);
	if (!StartSpotTag)
	{
		return nullptr;
	}

	UWorld* World = GetWorld();

	for (TActorIterator<APlayerStart> It(World); It; ++It)
	{
		if (It->PlayerStartTag == *StartSpotTag)
		{
			It->PlayerStartTag = FName("Taken");
			return *It;
		}
	}

	return nullptr;
}
