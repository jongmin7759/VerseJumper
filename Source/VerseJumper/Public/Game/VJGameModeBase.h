// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VJGameInstance.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
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
	void StartNewGame();
	UFUNCTION(BlueprintCallable)
	void ContinueGame();
	UFUNCTION(BlueprintCallable)
	void SaveGameSlot();
	UFUNCTION(BlueprintCallable)
	void DeleteSaveGameSlot();
	UFUNCTION(BlueprintCallable)
	UVJSaveGame* GetSaveGameData();
	UFUNCTION(BlueprintCallable)
	void UpdateWorldState(AActor* WorldActor);
	UFUNCTION(BlueprintCallable)
	void LoadWorldState(AActor* WorldActor);
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	
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
	UPROPERTY(EditDefaultsOnly)
	FName DefaultPlayerStartTag;

private:
	TWeakObjectPtr<UVJSaveGame> CurrentSaveGame;
};

