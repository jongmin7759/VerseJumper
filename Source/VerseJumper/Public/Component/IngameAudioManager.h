// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "IngameAudioManager.generated.h"

class UVerseStateSoundMap;
class AVJGameStateBase;

// Gamestate에 붙여서 인게임 상황에 대한 배경음 처리
// ex) VerseState전환 , 컷신 전환 등
UCLASS()
class VERSEJUMPER_API UIngameAudioManager : public UActorComponent
{
	GENERATED_BODY()

public:
	UIngameAudioManager();
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Audio")
	float FadeInTime = 0.f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Audio")
	float FadeOutTime = 0.f;
	UPROPERTY(EditDefaultsOnly,Category="Audio")
	TObjectPtr<USoundMix> MasterMix;
	UPROPERTY(EditDefaultsOnly,Category="Audio")
	TObjectPtr<USoundClass> InGameSoundClass;
	UPROPERTY(EditDefaultsOnly,Category="Audio")
	TObjectPtr<USoundSubmixBase> TrashVerseSubmix;
	//Save&Load (option)
	UPROPERTY(EditDefaultsOnly,Category="Audio")
	TObjectPtr<USoundClass> MasterSoundClass;
	UPROPERTY(EditDefaultsOnly,Category="Audio")
	TObjectPtr<USoundClass> AmbientSoundClass;
	UPROPERTY(EditDefaultsOnly,Category="Audio")
	TObjectPtr<USoundClass> CutSceneSoundClass;
	UPROPERTY(EditDefaultsOnly,Category="Audio")
	TObjectPtr<USoundClass> MusicSoundClass;
	UPROPERTY(EditDefaultsOnly,Category="Audio")
	TObjectPtr<USoundClass> SFXSoundClass;
	UPROPERTY(EditDefaultsOnly,Category="Audio")
	TObjectPtr<USoundClass> UISoundClass;

	UFUNCTION(BlueprintCallable)
	void ChangeTheme(FGameplayTag NewTheme,bool bUsingFadeOut = true);
	UFUNCTION(BlueprintCallable)
	void SetSoundClassVolume(USoundClass* InSoundClass, float InVolume, bool SaveOption = false);
	UFUNCTION(BlueprintCallable)
	void FadeOutInGameSound(float OverrideFadeOutTime = 0.f) const;
	UFUNCTION(BlueprintCallable)
	void FadeInInGameSound(float OverrideFadeInTime = 0.f) const;
	UFUNCTION(BlueprintCallable)
	void ResetAllVolume();

protected:
	virtual void BeginPlay() override;

private:
	FGameplayTag CurrentTheme;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UVerseStateSoundMap> VerseStateSoundMap;
	UPROPERTY()
	TObjectPtr<UAudioComponent> MusicComponent;
	UPROPERTY()
	TObjectPtr<UAudioComponent> FadeOutComponent;
	UPROPERTY()
	TObjectPtr<UAudioComponent> AmbientSoundComponent;

	void HandleStateChange(const FGameplayTag& NewState);
	void PlaySound(TObjectPtr<UAudioComponent>& AudioComponentRef, USoundBase* NewSound, const bool bUsingFadeOut = false);
	void ToggleSubmix(bool Activate);
	void LoadSaveGame();
};





