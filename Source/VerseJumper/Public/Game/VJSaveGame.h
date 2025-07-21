// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/SaveGame.h"
#include "VJSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class VERSEJUMPER_API UVJSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	// Player
	UPROPERTY()
	FTransform PlayerTransform;
	UPROPERTY()
	bool bHasModifier = false;
	UPROPERTY()
	FGameplayTagContainer SavedTags;

	// World
	UPROPERTY()
	int32 TestCounter = 0;

};
