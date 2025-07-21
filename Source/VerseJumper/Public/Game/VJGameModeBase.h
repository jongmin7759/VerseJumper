// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "VJGameModeBase.generated.h"

class UVJSaveGame;
class USaveGame;
/**
 * 
 */
UCLASS()
class VERSEJUMPER_API AVJGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void CreateGameSlot();
	UFUNCTION(BlueprintCallable)
	void SaveGameSlot();
	UFUNCTION(BlueprintCallable)
	void DeleteSaveGameSlot();
	UFUNCTION(BlueprintCallable)
	UVJSaveGame* GetSaveGameData();
	// For Debug
	UFUNCTION(BlueprintCallable)
	void UpdateCounter();

	
	UFUNCTION(BlueprintCallable)
	void TravelToGameLevel();
	UFUNCTION(BlueprintCallable)
	void TravelToMainMenuLevel();

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> GameLevel;
	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> MainMenuLevel;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USaveGame> SaveGameClass;

private:
	TWeakObjectPtr<UVJSaveGame> CurrentSaveGame;
};
