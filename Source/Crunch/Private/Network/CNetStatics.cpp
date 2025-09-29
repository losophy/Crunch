// Fill out your copyright notice in the Description page of Project Settings.


#include "Network/CNetStatics.h"

FOnlineSessionSettings UCNetStatics::GenerateOnlineSesisonSettings(const FName& SessionName, const FString& SessionSearchId, int Port)
{
	FOnlineSessionSettings OnlineSessionSettings{};
	OnlineSessionSettings.bIsLANMatch = false;
	OnlineSessionSettings.NumPublicConnections = GetPlayerCountPerTeam() * 2;
	OnlineSessionSettings.bShouldAdvertise = true;
	OnlineSessionSettings.bUsesPresence = false;
	OnlineSessionSettings.bAllowJoinViaPresence = false;
	OnlineSessionSettings.bAllowJoinViaPresenceFriendsOnly = false;
	OnlineSessionSettings.bAllowInvites = true;
	OnlineSessionSettings.bAllowJoinInProgress = false;
	OnlineSessionSettings.bUseLobbiesIfAvailable = false;
	OnlineSessionSettings.bUseLobbiesVoiceChatIfAvailable = false;
	OnlineSessionSettings.bUsesStats = true;

	OnlineSessionSettings.Set(GetSessionNameKey(), SessionName.ToString(), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	OnlineSessionSettings.Set(GetSessionSearchIdKey(), SessionSearchId, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	OnlineSessionSettings.Set(GetPortKey(), Port, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	return OnlineSessionSettings;
}

IOnlineSessionPtr UCNetStatics::GetSessionPtr()
{
	IOnlineSubsystem* OnlineSubSystem = IOnlineSubsystem::Get();
	if (OnlineSubSystem)
	{
		return OnlineSubSystem->GetSessionInterface();
	}
	return nullptr;
}

IOnlineIdentityPtr UCNetStatics::GetIdentityPtr()
{
	IOnlineSubsystem* OnlineSubSystem = IOnlineSubsystem::Get();
	if (OnlineSubSystem)
	{
		return OnlineSubSystem->GetIdentityInterface();
	}
	return nullptr;
}

uint8 UCNetStatics::GetPlayerCountPerTeam()
{
	return 5;
}

bool UCNetStatics::IsSessionServer(const UObject* WorldContextObject)
{
	return WorldContextObject->GetWorld()->GetNetMode() == ENetMode::NM_DedicatedServer;
}

FString UCNetStatics::GetSessionNameStr()
{
	return GetCommandlineArgAsString(GetSessionNameKey());
}

FName UCNetStatics::GetSessionNameKey()
{
	return FName("SESSION_NAME");
}

FString UCNetStatics::GetSesisonSearchIdStr()
{
	return GetCommandlineArgAsString(GetSessionSearchIdKey());
}

FName UCNetStatics::GetSessionSearchIdKey()
{
	return FName("SESSION_SEARCH_ID");
}

int UCNetStatics::GetSessionPort()
{
	return GetCommandlineArgAsInt(GetPortKey());
}

FName UCNetStatics::GetPortKey()
{
	return FName("PORT");
}

FString UCNetStatics::GetCommandlineArgAsString(const FName& ParamName)
{
	FString OutVal = "";
	FString CommandLineArg = FString::Printf(TEXT("%s="), *(ParamName.ToString()));
	FParse::Value(FCommandLine::Get(), *CommandLineArg, OutVal);
	return OutVal;
}

int UCNetStatics::GetCommandlineArgAsInt(const FName& ParamName)
{
	int OutVal = 0;
	FString CommandLineArg = FString::Printf(TEXT("%s="), *(ParamName.ToString()));
	FParse::Value(FCommandLine::Get(), *CommandLineArg, OutVal);
	return OutVal;
}
