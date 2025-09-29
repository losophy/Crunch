// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/CGameInstance.h"
#include "Network/CNetStatics.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Interfaces/OnlineIdentityInterface.h"

void UCGameInstance::StartMatch()
{
	if (GetWorld()->GetNetMode() == ENetMode::NM_DedicatedServer || GetWorld()->GetNetMode() == ENetMode::NM_ListenServer)
	{
		LoadLevelAndListen(GameLevel);
	}
}

void UCGameInstance::Init()
{
	Super::Init();
	if (GetWorld()->IsEditorWorld())
		return;

	if (UCNetStatics::IsSessionServer(this))
	{
		CreateSession();
	}
}

void UCGameInstance::CreateSession()
{
	IOnlineSessionPtr SessionPtr = UCNetStatics::GetSessionPtr();
	if (SessionPtr)
	{
		ServerSesisonName = UCNetStatics::GetSessionNameStr();
		FString SessionSearchId = UCNetStatics::GetSesisonSearchIdStr();
		SessionServerPort = UCNetStatics::GetSessionPort();
		UE_LOG(LogTemp, Warning, TEXT("#### Create Session With Name: %s, ID: %s, Port: %d"), *(ServerSesisonName), *(SessionSearchId), SessionServerPort)

		FOnlineSessionSettings OnlineSesisonSetting = UCNetStatics::GenerateOnlineSesisonSettings(FName(ServerSesisonName), SessionSearchId, SessionServerPort);
		if (!SessionPtr->CreateSession(0, FName(ServerSesisonName), OnlineSesisonSetting))
		{
			UE_LOG(LogTemp, Warning, TEXT("Sesison Creating Failed Right away!!!!"))
		}
	}
}

void UCGameInstance::LoadLevelAndListen(TSoftObjectPtr<UWorld> Level)
{
	const FName LevelURL = FName(*FPackageName::ObjectPathToPackageName(Level.ToString()));

	if (LevelURL != "")
	{
		GetWorld()->ServerTravel(LevelURL.ToString() + "?listen");
	}
}
