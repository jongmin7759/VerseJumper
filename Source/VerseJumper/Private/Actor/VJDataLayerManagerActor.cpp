// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/VJDataLayerManagerActor.h"

#include "Kismet/GameplayStatics.h"
#include "Subsystem/VerseStateSubsystem.h"
#include "WorldPartition/DataLayer/DataLayerSubsystem.h"

// Sets default values
AVJDataLayerManagerActor::AVJDataLayerManagerActor()
{
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AVJDataLayerManagerActor::BeginPlay()
{
	Super::BeginPlay();

	// OnVerseStateChanged 바인딩
	UVerseStateSubsystem* VerseStateSubsystem = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UVerseStateSubsystem>();
	checkf(VerseStateSubsystem,TEXT("VerseStateSubsystem was NULL when tries to Bind OnVerseStateChanged"));
	VerseStateSubsystem->VerseStateChanged.AddUObject(this,&AVJDataLayerManagerActor::Internal_OnVerseStateChanged);
}

void AVJDataLayerManagerActor::Internal_OnVerseStateChanged(const FGameplayTag& NewState)
{
	if (!StateLayerMap.Contains(NewState))
	{
		UE_LOG(LogTemp,Error,TEXT("DataLayerManagerActor : Invalid State"));
		return;
	}
	const UDataLayerAsset* DataLayerToActivate = *StateLayerMap.Find(NewState);
	if (DataLayerToActivate == nullptr)
	{
		UE_LOG(LogTemp,Error,TEXT("DataLayerManagerActor : No DataLayer found"));
		return;
	}
	if (UDataLayerManager* DataLayerManager = UDataLayerManager::GetDataLayerManager(GetWorld()))
	{
		// 이미 적용된 데이터레이어가 있다면 언로드하고 현재 레이어 갱신하기
		if (CurrentDataLayer)
		{
			DataLayerManager->SetDataLayerInstanceRuntimeState(DataLayerManager->GetDataLayerInstance(CurrentDataLayer), EDataLayerRuntimeState::Unloaded);
		}
		DataLayerManager->SetDataLayerInstanceRuntimeState(DataLayerManager->GetDataLayerInstance(DataLayerToActivate), EDataLayerRuntimeState::Activated);
		CurrentDataLayer = DataLayerToActivate;
	}
}
