// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/LobbyGameMode.h"
#include "Network/CGameSession.h"

ALobbyGameMode::ALobbyGameMode()
{
	bUseSeamlessTravel = true;
	GameSessionClass = ACGameSession::StaticClass();
}
