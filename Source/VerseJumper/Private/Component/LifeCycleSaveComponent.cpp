// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/LifeCycleSaveComponent.h"

#include "Game/VJGameModeBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
ULifeCycleSaveComponent::ULifeCycleSaveComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}


void ULifeCycleSaveComponent::BeginPlay()
{
	Super::BeginPlay();

	LoadOwnerActor();
}

void ULifeCycleSaveComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	// 단순 게임 종료할 때는 세이브 업데이트할 필요 없음
	if (EndPlayReason == EEndPlayReason::Quit) return;

	// 스트리밍 아웃될 때 자동으로 세이브 업데이트할 수 있도록 (보통은 수동 세이브시킬 것)
	if (bNeedSave) SaveOwnerActor();
}

void ULifeCycleSaveComponent::SaveOwnerActor()
{
	// Save Owner Actor
	if (AVJGameModeBase* VJGameMode = Cast<AVJGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		VJGameMode->UpdateWorldState(GetOwner());
		OnSaved.Broadcast();
	}
}

void ULifeCycleSaveComponent::LoadOwnerActor()
{
	// Load Owner Actor
	if (AVJGameModeBase* VJGameMode = Cast<AVJGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		VJGameMode->LoadWorldState(GetOwner());
		OnLoaded.Broadcast();
	}
}
