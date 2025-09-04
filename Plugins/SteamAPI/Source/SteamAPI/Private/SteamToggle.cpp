// Fill out your copyright notice in the Description page of Project Settings.

#include "SteamToggle.h"

namespace
{
	static bool GInitialized = false;
	static bool GUseSteam = true;

	static bool ParseUseSteamFromCmd()
	{
		const TCHAR* Cmd = FCommandLine::Get();
		const bool bNoSteam  = FParse::Param(Cmd, TEXT("nosteam"));
		if (bNoSteam) return false;
		return true; // 기본 ON
	}
}

namespace SteamToggle
{
	void InitializeEarly()
	{
		if (GInitialized) return;
		GUseSteam    = ParseUseSteamFromCmd();
		GInitialized = true;

		UE_LOG(LogTemp, Log, TEXT("[SteamToggle] %s  (cmd: %s)"),
			GUseSteam ? TEXT("ENABLED") : TEXT("DISABLED"),
			FCommandLine::Get());
	}

	bool IsUseSteam()
	{
		if (!GInitialized) InitializeEarly();
		return GUseSteam;
	}
}