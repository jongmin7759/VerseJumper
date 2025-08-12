#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "OptionsSaveGame.generated.h"

/**
 * Options 저장  
 */
UCLASS()
class VERSEJUMPER_API UOptionsSaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	// Controls
	UPROPERTY(BlueprintReadOnly)
	float MouseSensitivity = 1.f;
	UPROPERTY(BlueprintReadOnly)
	bool bInvertY = false;

	// Audio
	UPROPERTY(BlueprintReadOnly)
	float MasterVolume = 1.f;
	UPROPERTY(BlueprintReadOnly)
	float AmbientVolume = 1.f;
	UPROPERTY(BlueprintReadOnly)
	float CutSceneVolume = 1.f;
	UPROPERTY(BlueprintReadOnly)
	float MusicVolume = 1.f;
	UPROPERTY(BlueprintReadOnly)
	float SFXVolume = 1.f;
	UPROPERTY(BlueprintReadOnly)
	float UIVolume = 1.f;
};
