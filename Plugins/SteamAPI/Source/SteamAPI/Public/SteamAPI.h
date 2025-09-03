// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

#pragma warning(push)
#pragma warning(disable : 4996)
#include "SteamAPI/Steam/steam_api.h"
#pragma warning(pop)

#define RAW_STEAM_APP_ID "3994420"

class FSteamAPIModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	// STEAM
	static constexpr const char* STEAM_APP_ID = RAW_STEAM_APP_ID;
};


//Steam → IETF/UE 문화코드 매핑 테이블
static FString MapSteamToIETF(const FString& InSteam)
{
	const FString S = InSteam.ToLower();
	static const TMap<FString, FString> M = {
		{TEXT("english"),   TEXT("en")},
		{TEXT("koreana"),   TEXT("ko-KR")},
	};
	if (const FString* Found = M.Find(S)) return *Found;
	return FString(); // 미지원 토큰이면 빈 값
}