// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SteamAPI.h"

// Util 함수만 사용할 거니까 따로 클래스 안 만들기
namespace SteamToggle
{
	// 가장 이른 시점(플러그인 StartupModule)에서 1회 호출
	STEAMAPI_API void InitializeEarly();

	// 어디서나 읽기 (플러그인/게임 공용)
	STEAMAPI_API bool IsUseSteam();
}