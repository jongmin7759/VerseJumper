// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/IngameAudioManager.h"

#include "Components/AudioComponent.h"
#include "DataAsset/VerseStateSoundMap.h"
#include "Game/OptionsSaveGame.h"
#include "Game/VJGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystem/VerseStateSubsystem.h"

UIngameAudioManager::UIngameAudioManager()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UIngameAudioManager::BeginPlay()
{
	Super::BeginPlay();

	LoadSaveGame();
	
	// OnVerseStateChanged 바인딩
	UVerseStateSubsystem* VerseStateSubsystem = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UVerseStateSubsystem>();
	checkf(VerseStateSubsystem,TEXT("VerseStateSubsystem was NULL when tries to Bind OnVerseStateChanged"));
	VerseStateSubsystem->VerseStateChanged.AddUObject(this,&UIngameAudioManager::HandleStateChange);
	HandleStateChange(VerseStateSubsystem->GetCurrentState());
}

void UIngameAudioManager::HandleStateChange(const FGameplayTag& NewState)
{
	if (VerseStateSoundMap == nullptr) return;

	// Submix
	const FGameplayTag TrashVerse = FGameplayTag::RequestGameplayTag("VerseState.TrashVerse");
	ToggleSubmix(NewState.MatchesTag(TrashVerse));
	
	//PlayAmbient
	PlaySound(AmbientSoundComponent,VerseStateSoundMap->GetAmbientSound(NewState));
}

void UIngameAudioManager::PlaySound(TObjectPtr<UAudioComponent>& AudioComponentRef, USoundBase* NewSound, const bool bUsingFadeOut)
{
	if (NewSound == nullptr) return;

	if (AudioComponentRef->IsValidLowLevel())
	{
		if (bUsingFadeOut)
		{
			FadeOutComponent = AudioComponentRef;
			FadeOutComponent->FadeOut(FadeOutTime,0.f);
		}
		else
		{
			AudioComponentRef->Stop();
		}
	}
	AudioComponentRef = UGameplayStatics::SpawnSound2D(this,NewSound);
	
	if (AudioComponentRef->IsValidLowLevel())
	{
		AudioComponentRef->FadeIn(FadeInTime);
	}
}

void UIngameAudioManager::ToggleSubmix(bool Activate)
{
	if (MusicComponent == nullptr) return;
	
	//Submix 적용
	if (Activate)
	{
		MusicComponent->SetSubmixSend(TrashVerseSubmix,1);
	}
	else
	{
		MusicComponent->SetSubmixSend(TrashVerseSubmix,0);
	}
}

void UIngameAudioManager::LoadSaveGame()
{
	AVJGameModeBase* VJGameMode = Cast<AVJGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (VJGameMode)
	{
		UOptionsSaveGame* SaveData = VJGameMode->GetOptionsSaveGameData();
		if (SaveData == nullptr) return;

		SetSoundClassVolume(MasterSoundClass,SaveData->MasterVolume);
		SetSoundClassVolume(AmbientSoundClass,SaveData->AmbientVolume);
		SetSoundClassVolume(CutSceneSoundClass,SaveData->CutSceneVolume);
		SetSoundClassVolume(MusicSoundClass,SaveData->MusicVolume);
		SetSoundClassVolume(SFXSoundClass,SaveData->SFXVolume);
		SetSoundClassVolume(UISoundClass,SaveData->UIVolume);
	}
}


void UIngameAudioManager::ChangeTheme(FGameplayTag NewTheme)
{
	if (NewTheme.MatchesTag(CurrentTheme)) return;

	CurrentTheme = NewTheme;
	PlaySound(MusicComponent,VerseStateSoundMap->GetMusic(NewTheme),true);

	if (UVerseStateSubsystem* VerseStateSubsystem = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UVerseStateSubsystem>())
	{
		const FGameplayTag CurrentState = VerseStateSubsystem->GetCurrentState();
		const FGameplayTag TrashVerse = FGameplayTag::RequestGameplayTag("VerseState.TrashVerse");
		ToggleSubmix(CurrentState.MatchesTag(TrashVerse));
	}
}

void UIngameAudioManager::SetSoundClassVolume(USoundClass* InSoundClass, float InVolume, bool SaveOption)
{
	UGameplayStatics::SetSoundMixClassOverride(this,MasterMix,InSoundClass,InVolume,1);
	AVJGameModeBase* VJGameMode = Cast<AVJGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (VJGameMode && SaveOption)
	{
		UOptionsSaveGame* Options = VJGameMode->GetOptionsSaveGameData();
		if (InSoundClass == MasterSoundClass) Options->MasterVolume = InVolume;
		else if (InSoundClass == CutSceneSoundClass) Options->CutSceneVolume = InVolume;
		else if (InSoundClass == MusicSoundClass) Options->MusicVolume = InVolume;
		else if (InSoundClass == SFXSoundClass) Options->SFXVolume = InVolume;
		else if (InSoundClass == UISoundClass) Options->UIVolume = InVolume;
		else if (InSoundClass == AmbientSoundClass) Options->AmbientVolume = InVolume;
		
		VJGameMode->SaveOptions();
	}
}

void UIngameAudioManager::FadeOutInGameSound(float OverrideFadeOutTime) const
{
	const float NewFadeOutTime = OverrideFadeOutTime <= 0.f ? FadeOutTime : OverrideFadeOutTime;
	// IngameSound 믹스를 0으로
	UGameplayStatics::SetSoundMixClassOverride(this,MasterMix,InGameSoundClass,0.4,1,NewFadeOutTime);
}

void UIngameAudioManager::FadeInInGameSound(float OverrideFadeInTime) const
{
	// IngameSound 다시 원래대로
	const float NewFadeInTime = OverrideFadeInTime <= 0.f ? FadeOutTime : OverrideFadeInTime;
	// IngameSound 믹스를 0으로
	UGameplayStatics::SetSoundMixClassOverride(this,MasterMix,InGameSoundClass,1,1,NewFadeInTime);
}

void UIngameAudioManager::ResetAllVolume()
{
	SetSoundClassVolume(MasterSoundClass,1.f,true);
	SetSoundClassVolume(AmbientSoundClass,1.f,true);
	SetSoundClassVolume(CutSceneSoundClass,1.f,true);
	SetSoundClassVolume(MusicSoundClass,1.f,true);
	SetSoundClassVolume(SFXSoundClass,1.f,true);
	SetSoundClassVolume(UISoundClass,1.f,true);
}