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

	UFUNCTION(BlueprintPure)
	bool DoesSaveExist() const;

	UPROPERTY()
	FName PlayerStartTag = FName();

	UPROPERTY()
	FString LoadSlotName = FString("SaveGame");
	UPROPERTY()
	FString OptionsSlotName = FString("OptionsSaveGame");


	UPROPERTY()
	int32 LoadSlotIndex = 0;

};
