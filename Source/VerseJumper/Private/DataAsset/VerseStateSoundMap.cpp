// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAsset/VerseStateSoundMap.h"

USoundBase* UVerseStateSoundMap::GetMusic(FGameplayTag State) const
{
	if (StateInfoMap.Contains(State))
	{
		return StateInfoMap[State].Music;
	}
	
	return nullptr;
}

USoundBase* UVerseStateSoundMap::GetAmbientSound(FGameplayTag State) const
{
	if (StateInfoMap.Contains(State))
	{
		return StateInfoMap[State].AmbientSound;
	}
	
	return nullptr;
}
