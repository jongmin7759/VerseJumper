// Copyright Epic Games, Inc. All Rights Reserved.

#include "SteamAPI.h"

#define LOCTEXT_NAMESPACE "FSteamAPIModule"

void FSteamAPIModule::StartupModule()
{
	// Steam Init
	FFileHelper::SaveStringToFile(TEXT(RAW_STEAM_APP_ID),TEXT("steam_appid.txt"));
	//E:\EpicGames\UE_5.5\Engine\Binaries\Win64에 steam_appid 파일 사용 (테스트용)
	if (SteamAPI_RestartAppIfNecessary(atoi(STEAM_APP_ID)))
	{
		UE_LOG(LogTemp, Error, TEXT("Steam API Restart"));
		FPlatformMisc::RequestExit(true); // 즉시 종료
		return;
	}

	if (!SteamAPI_Init())
	{
		UE_LOG(LogTemp, Error, TEXT("SteamAPI_Init Return False"));
	}
}

void FSteamAPIModule::ShutdownModule()
{
	//Steam
	SteamAPI_Shutdown();
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSteamAPIModule, SteamAPI)