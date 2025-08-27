// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VJGameInstance.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "VJGameModeBase.generated.h"

class UOptionsSaveGame;
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
	UFUNCTION(BlueprintPure)
	bool IsNewGame() const;
	UFUNCTION(BlueprintCallable)
	void StartNewGame();
	UFUNCTION(BlueprintCallable)
	void ContinueGame();
	UFUNCTION(BlueprintCallable)
	void SaveGameSlot();
	//Options
	UFUNCTION(BlueprintCallable)
	void SaveMouseSensitivity(float NewValue);
	UFUNCTION(BlueprintCallable)
	void SaveInvertY(bool NewValue);
	UFUNCTION(BlueprintCallable)
	void SaveOptions();
	UFUNCTION(BlueprintCallable)
	void DeleteSaveGameSlot();
	UFUNCTION(BlueprintCallable)
	UVJSaveGame* GetSaveGameData();
	UFUNCTION(BlueprintCallable)
	UOptionsSaveGame* GetOptionsSaveGameData();
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
	TSubclassOf<USaveGame> OptionsSaveGameClass;
	UPROPERTY(EditDefaultsOnly)
	FName DefaultPlayerStartTag;

private:
	TWeakObjectPtr<UVJSaveGame> CurrentSaveGame;
	TWeakObjectPtr<UOptionsSaveGame> CurrentOptionsSaveGame;

};

