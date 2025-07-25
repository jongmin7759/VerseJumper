// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/IngameAudioManager.h"

#include "Game/VJGameStateBase.h"
#include "Components/AudioComponent.h"
#include "DataAsset/VerseStateSoundMap.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystem/VerseStateSubsystem.h"

UIngameAudioManager::UIngameAudioManager()
{
	PrimaryComponentTick.bCanEverTick = false;

	VJGameStateBase = Cast<AVJGameStateBase>(GetOwner());
}

void UIngameAudioManager::BeginPlay()
{
	Super::BeginPlay();

	// OnVerseStateChanged 바인딩
	UVerseStateSubsystem* VerseStateSubsystem = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UVerseStateSubsystem>();
	checkf(VerseStateSubsystem,TEXT("VerseStateSubsystem was NULL when tries to Bind OnVerseStateChanged"));
	VerseStateSubsystem->VerseStateChanged.AddUObject(this,&UIngameAudioManager::HandleStateChange);
}

void UIngameAudioManager::HandleStateChange(const FGameplayTag& NewState)
{
	//PlayMusic (크로스페이드)
	PlaySound(MusicComponent,VerseStateSoundMap->GetMusic(NewState),true);
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

void UIngameAudioManager::FadeOutInGameSound(float OverrideFadeOutTime) const
{
	const float NewFadeOutTime = OverrideFadeOutTime <= 0.f ? FadeOutTime : OverrideFadeOutTime;
	// IngameSound 믹스를 0으로
	UGameplayStatics::SetSoundMixClassOverride(this,MasterMix,InGameSoundClass,0,1,NewFadeOutTime);
}

void UIngameAudioManager::FadeInInGameSound(float OverrideFadeInTime) const
{
	// IngameSound 다시 원래대로
	const float NewFadeInTime = OverrideFadeInTime <= 0.f ? FadeOutTime : OverrideFadeInTime;
	// IngameSound 믹스를 0으로
	UGameplayStatics::SetSoundMixClassOverride(this,MasterMix,InGameSoundClass,1,1,NewFadeInTime);
}
