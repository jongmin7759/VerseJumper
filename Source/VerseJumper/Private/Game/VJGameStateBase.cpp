// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/VJGameStateBase.h"

#include "Component/IngameAudioManager.h"

AVJGameStateBase::AVJGameStateBase()
{
	InGameAudioManager = CreateDefaultSubobject<UIngameAudioManager>("InGameAudioManager");
}
