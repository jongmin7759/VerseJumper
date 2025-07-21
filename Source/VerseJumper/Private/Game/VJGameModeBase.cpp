// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/VJGameModeBase.h"

#include "Game/VJGameInstance.h"
#include "Game/VJSaveGame.h"
#include "Kismet/GameplayStatics.h"

void AVJGameModeBase::CreateGameSlot()
{
	UVJGameInstance* VJGameInstance = Cast<UVJGameInstance>(UGameplayStatics::GetGameInstance(this));
	const FString LoadSlotName = VJGameInstance->LoadSlotName;
	const int32 LoadSlotIndex = VJGameInstance->LoadSlotIndex;

	USaveGame* SaveGameObject = UGameplayStatics::CreateSaveGameObject(SaveGameClass);
	UVJSaveGame* VJSaveGame = Cast<UVJSaveGame>(SaveGameObject);

	UGameplayStatics::SaveGameToSlot(VJSaveGame, LoadSlotName, LoadSlotIndex);
}

void AVJGameModeBase::SaveGameSlot()
{
	UVJGameInstance* VJGameInstance = Cast<UVJGameInstance>(UGameplayStatics::GetGameInstance(this));
	const FString LoadSlotName = VJGameInstance->LoadSlotName;
	const int32 LoadSlotIndex = VJGameInstance->LoadSlotIndex;

	UGameplayStatics::SaveGameToSlot(GetSaveGameData(), LoadSlotName, LoadSlotIndex);
}

void AVJGameModeBase::DeleteSaveGameSlot()
{
	UVJGameInstance* VJGameInstance = Cast<UVJGameInstance>(UGameplayStatics::GetGameInstance(this));
	const FString LoadSlotName = VJGameInstance->LoadSlotName;
	const int32 LoadSlotIndex = VJGameInstance->LoadSlotIndex;
	
	if (UGameplayStatics::DoesSaveGameExist(LoadSlotName, LoadSlotIndex))
	{
		UGameplayStatics::DeleteGameInSlot(LoadSlotName, LoadSlotIndex);
	}
}

UVJSaveGame* AVJGameModeBase::GetSaveGameData()
{
	if (!CurrentSaveGame.IsValid())
	{
		UVJGameInstance* VJGameInstance = Cast<UVJGameInstance>(UGameplayStatics::GetGameInstance(this));
		const FString LoadSlotName = VJGameInstance->LoadSlotName;
		const int32 LoadSlotIndex = VJGameInstance->LoadSlotIndex;
	
		USaveGame* SaveGameObject = nullptr;
		if (UGameplayStatics::DoesSaveGameExist(LoadSlotName, LoadSlotIndex))
		{
			SaveGameObject = UGameplayStatics::LoadGameFromSlot(LoadSlotName, LoadSlotIndex);
		}
		else
		{
			SaveGameObject = UGameplayStatics::CreateSaveGameObject(SaveGameClass);
		}
		CurrentSaveGame = Cast<UVJSaveGame>(SaveGameObject);		
	}
	
	return CurrentSaveGame.Get();
}

void AVJGameModeBase::UpdateCounter()
{
	AVJGameModeBase* VJGameMode = Cast<AVJGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (VJGameMode)
	{
		UVJSaveGame* SaveData = VJGameMode->GetSaveGameData();
		if (SaveData == nullptr) return;

		SaveData->TestCounter++;

		//VJGameMode->SaveGameSlot(SaveData);
	}
}

void AVJGameModeBase::TravelToGameLevel()
{
	// 싱글 게임이니까 게임모드 넘겨도 되긴하지만 멀티플레이 게임의 경우 주의할 것 (서버에만 존재)
	UGameplayStatics::OpenLevelBySoftObjectPtr(this, GameLevel);
}

void AVJGameModeBase::TravelToMainMenuLevel()
{
	UGameplayStatics::OpenLevelBySoftObjectPtr(this, MainMenuLevel);
}
