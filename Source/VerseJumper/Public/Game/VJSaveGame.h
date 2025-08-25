// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/SaveGame.h"
#include "VJSaveGame.generated.h"

USTRUCT()
struct FSavedActor
{
	GENERATED_BODY()

	UPROPERTY()
	FName ActorName = FName();

	UPROPERTY()
	FTransform Transform = FTransform();

	// SaveGame 속성 사용해서 시리얼라이즈
	UPROPERTY()
	TArray<uint8> Bytes;
};

inline bool operator==(const FSavedActor& Left, const FSavedActor& Right)
{
	return Left.ActorName == Right.ActorName;
}

UCLASS()
class VERSEJUMPER_API UVJSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	// Player
	UPROPERTY()
	FName PlayerStartTag;
	UPROPERTY()
	bool bHasModifier = false;
	UPROPERTY()
	bool bHasCardKey = false;
	UPROPERTY()
	float ElapsedTime = 0.f;
	UPROPERTY()
	FGameplayTagContainer SavedTags;
	UPROPERTY()
	TSet<FName> CollectedIDs;
	UPROPERTY()
	int32 TotalCollected = 0;
	UPROPERTY()
	FGameplayTag CurrentState = FGameplayTag::RequestGameplayTag("VerseState.AlphaVerse");
	UPROPERTY()
	FGameplayTag CurrentStage = FGameplayTag();
	UPROPERTY()
	TArray<FGameplayTag> AvailableStates = {FGameplayTag::RequestGameplayTag("VerseState.AlphaVerse")};

	// World
	UPROPERTY()
	TMap<FName,FSavedActor> SavedActors;

};
