// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "VerseStateSoundMap.generated.h"

USTRUCT(BlueprintType)
struct FVerseStateSoundInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> Music = nullptr;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> AmbientSound = nullptr;
};

UCLASS()
class VERSEJUMPER_API UVerseStateSoundMap : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TMap<FGameplayTag,FVerseStateSoundInfo> StateInfoMap;

	// State에 대응하는 Music 반환
	USoundBase* GetMusic(FGameplayTag State) const;
	// State에 대응하는 AmbientSound 반환
	USoundBase* GetAmbientSound(FGameplayTag State) const;
};
