// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/VJGameModeBase.h"

#include "Game/VJGameInstance.h"
#include "Game/VJSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

void AVJGameModeBase::StartNewGame()
{
	UVJGameInstance* VJGameInstance = Cast<UVJGameInstance>(UGameplayStatics::GetGameInstance(this));
	const FString LoadSlotName = VJGameInstance->LoadSlotName;
	const int32 LoadSlotIndex = VJGameInstance->LoadSlotIndex;
	VJGameInstance->PlayerStartTag = DefaultPlayerStartTag;

	// 기존 세이브 지우고
	DeleteSaveGameSlot();
	USaveGame* SaveGameObject = UGameplayStatics::CreateSaveGameObject(SaveGameClass);
	UVJSaveGame* VJSaveGame = Cast<UVJSaveGame>(SaveGameObject);
	// 새로 생성
	UGameplayStatics::SaveGameToSlot(VJSaveGame, LoadSlotName, LoadSlotIndex);
}

void AVJGameModeBase::ContinueGame()
{
	UVJGameInstance* VJGameInstance = Cast<UVJGameInstance>(UGameplayStatics::GetGameInstance(this));

	// 기존 세이브 데이터에서 플레이어 스타트 태그만 받아오면 됨
	CurrentSaveGame = GetSaveGameData();
	VJGameInstance->PlayerStartTag = CurrentSaveGame->PlayerStartTag;
}

void AVJGameModeBase::SaveGameSlot()
{
	UVJGameInstance* VJGameInstance = Cast<UVJGameInstance>(UGameplayStatics::GetGameInstance(this));
	const FString LoadSlotName = VJGameInstance->LoadSlotName;
	const int32 LoadSlotIndex = VJGameInstance->LoadSlotIndex;

	CurrentSaveGame = GetSaveGameData();
	VJGameInstance->PlayerStartTag = CurrentSaveGame->PlayerStartTag;
	
	UGameplayStatics::SaveGameToSlot(CurrentSaveGame.Get(), LoadSlotName, LoadSlotIndex);
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

void AVJGameModeBase::UpdateWorldState(AActor* WorldActor)
{
	UVJSaveGame* SaveData = GetSaveGameData();
	if (SaveData == nullptr) return;

	FSavedActor SavedActor;
	SavedActor.ActorName = WorldActor->GetFName();
	SavedActor.Transform = WorldActor->GetTransform();

	FMemoryWriter MemoryWriter(SavedActor.Bytes);

	FObjectAndNameAsStringProxyArchive Archive(MemoryWriter, true);
	Archive.ArIsSaveGame = true;

	WorldActor->Serialize(Archive);
	FSavedActor& ActorToUpdate = SaveData->SavedActors.FindOrAdd(SavedActor.ActorName);
	ActorToUpdate = SavedActor;
}

void AVJGameModeBase::LoadWorldState(AActor* WorldActor)
{
	UVJGameInstance* VJGI = Cast<UVJGameInstance>(GetGameInstance());
	check(VJGI);
	if (UGameplayStatics::DoesSaveGameExist(VJGI->LoadSlotName, VJGI->LoadSlotIndex))
	{

		UVJSaveGame* SaveData = GetSaveGameData();
		if (SaveData == nullptr) return;
		
		if (SaveData->SavedActors.Contains(WorldActor->GetFName()))
		{
			const FSavedActor& ActorToLoad = SaveData->SavedActors.FindRef(WorldActor->GetFName());
			FMemoryReader MemoryReader(ActorToLoad.Bytes);

			FObjectAndNameAsStringProxyArchive Archive(MemoryReader, true);
			Archive.ArIsSaveGame = true;
			WorldActor->Serialize(Archive); 

			// TODO : SaveInterface 만들어서 트랜스폼 수정 필요한 경우만 로드할 수 있도록하기 (일단 트랜스폼 변경 없이)
			//WorldActor->SetActorTransform(ActorToLoad.Transform);

		}
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


AActor* AVJGameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{
	UVJGameInstance* VJGameInstance = Cast<UVJGameInstance>(GetGameInstance());
	
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), Actors);
	if (Actors.Num() > 0)
	{
		AActor* SelectedActor = Actors[0];
		for (AActor* Actor : Actors)
		{
			if (APlayerStart* PlayerStart = Cast<APlayerStart>(Actor))
			{
				if (PlayerStart->PlayerStartTag == VJGameInstance->PlayerStartTag)
				{
					SelectedActor = PlayerStart;
					break;
				}
			}
		}
		return SelectedActor;
	}
	return nullptr;
}