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

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> LoadingWidgetClass;
	UPROPERTY()
	TObjectPtr<UUserWidget> LoadingWidget;
private:
	void OnPreLoadMap(const FString& MapName);
	void OnPostLoadMap(UWorld* World);

};
