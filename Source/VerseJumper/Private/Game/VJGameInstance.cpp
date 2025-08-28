// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/VJGameInstance.h"

#include "GameFramework/GameUserSettings.h"
#include "Kismet/GameplayStatics.h"

void UVJGameInstance::Init()
{
	Super::Init();

	FString Culture;
	
	if (!GConfig->GetString(TEXT("Internationalization"), TEXT("Culture"), Culture, GGameUserSettingsIni))
	{
		GConfig->GetString(TEXT("Internationalization"), TEXT("Culture"), Culture, GGameIni);
	}

	FInternationalization::Get().SetCurrentCulture(Culture);

	if (GEngine)
	{
		if (UGameUserSettings* GS = GEngine->GetGameUserSettings())
		{
			GS->LoadSettings();
			GS->ApplySettings(false);
		}
	}
}

bool UVJGameInstance::DoesSaveExist() const
{
	return UGameplayStatics::DoesSaveGameExist(LoadSlotName, LoadSlotIndex);
}
