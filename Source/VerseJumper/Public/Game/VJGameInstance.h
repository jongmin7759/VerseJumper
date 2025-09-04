// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "VJGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class VERSEJUMPER_API UVJGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	void Init() override;
	void Shutdown() override;

	// Instance Init에서 부르면 너무 이른듯해서 게임모드에서 부름
	void InitCulture();
	
	UFUNCTION(BlueprintPure)
	bool DoesSaveExist() const;

	UFUNCTION(BlueprintCallable)
	void ShowLoadingScreen();
	UFUNCTION(BlueprintCallable)
	void HideLoadingScreen();

	UPROPERTY()
	FName PlayerStartTag = FName();

	UPROPERTY()
	FString LoadSlotName = FString("SaveGame");
	UPROPERTY()
	FString OptionsSlotName = FString("OptionsSaveGame");

	UPROPERTY()
	int32 LoadSlotIndex = 0;

	UFUNCTION(BlueprintPure)
	FString GetLauncherGameLanguage() const;
	
protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> LoadingWidgetClass;
private:
	void OnPreLoadMap(const FString& MapName);
	void OnPostLoadMap(UWorld* World);
	bool bAlreadyInit = false;
};
