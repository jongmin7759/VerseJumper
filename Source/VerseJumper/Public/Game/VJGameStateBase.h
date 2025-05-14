// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "VJGameStateBase.generated.h"

class UIngameAudioManager;
/**
 * 
 */
UCLASS()
class VERSEJUMPER_API AVJGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	AVJGameStateBase();
protected:
	UPROPERTY(VisibleAnywhere,Category="Sound")
	TObjectPtr<UIngameAudioManager> InGameAudioManager;
	
	
};
