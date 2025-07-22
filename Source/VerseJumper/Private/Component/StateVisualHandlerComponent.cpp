// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/StateVisualHandlerComponent.h"

#include "DataAsset/VerseStateVisualMap.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialParameterCollectionInstance.h"
#include "Subsystem/VerseStateSubsystem.h"
#include "WorldPartition/DataLayer/DataLayerManager.h"

UStateVisualHandlerComponent::UStateVisualHandlerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}


void UStateVisualHandlerComponent::BeginPlay()
{
	Super::BeginPlay();

	// OnVerseStateChanged 바인딩
	UVerseStateSubsystem* VerseStateSubsystem = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UVerseStateSubsystem>();
	checkf(VerseStateSubsystem,TEXT("VerseStateSubsystem was NULL when tries to Bind OnVerseStateChanged"));
	VerseStateSubsystem->VerseStateChanged.AddUObject(this,&UStateVisualHandlerComponent::HandleStateChange);

	// VerseStateVisualMap 정보가 없으면 작동 안 함
	checkf(VerseStateVisualMap,TEXT("VerseStateVisualMap was not allocated"));
	// MPC 할당 필요
	checkf(LandscapeMPC,TEXT("LandscapeMPC was not allocated"));

	InitDataLayers();
}

void UStateVisualHandlerComponent::InitDataLayers()
{
	TArray<FGameplayTag> AllStates;
	VerseStateVisualMap->StateInfoMap.GetKeys(AllStates);

	UDataLayerManager* DataLayerManager = UDataLayerManager::GetDataLayerManager(GetWorld());
	if (DataLayerManager == nullptr) return ;
	
	for (const FGameplayTag& State : AllStates)
	{
		const UDataLayerAsset* DataLayerToActivate = VerseStateVisualMap->GetWorldPartitionLayer(State);
		if (DataLayerToActivate == nullptr)
		{
			UE_LOG(LogTemp,Error,TEXT("UStateVisualHandlerComponent : No DataLayer found"));
			continue;
		}
		DataLayerManager->SetDataLayerInstanceRuntimeState(DataLayerManager->GetDataLayerInstance(DataLayerToActivate), EDataLayerRuntimeState::Loaded);
	}
	
	if (UVerseStateSubsystem* VerseStateSubsystem = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UVerseStateSubsystem>())
	{
		HandleStateChange(VerseStateSubsystem->GetCurrentState());
	}
}

void UStateVisualHandlerComponent::HandleStateChange(const FGameplayTag& NewState)
{
	HandleWorldPartitionLayerVisibility(NewState);
	HandleMPC(NewState);
}

void UStateVisualHandlerComponent::HandleWorldPartitionLayerVisibility(const FGameplayTag& NewState)
{
	const UDataLayerAsset* DataLayerToActivate = VerseStateVisualMap->GetWorldPartitionLayer(NewState);

	if (DataLayerToActivate == nullptr)
	{
		UE_LOG(LogTemp,Error,TEXT("UStateVisualHandlerComponent : No DataLayer found"));
		return;
	}
	if (CurrentDataLayer == DataLayerToActivate) return;
	
	if (UDataLayerManager* DataLayerManager = UDataLayerManager::GetDataLayerManager(GetWorld()))
	{
		// 이미 적용된 데이터레이어가 있다면 Visibility 끄고 현재 레이어 갱신하기
		if (CurrentDataLayer)
		{
			DataLayerManager->SetDataLayerInstanceRuntimeState(DataLayerManager->GetDataLayerInstance(CurrentDataLayer), EDataLayerRuntimeState::Loaded);
		}
		DataLayerManager->SetDataLayerInstanceRuntimeState(DataLayerManager->GetDataLayerInstance(DataLayerToActivate), EDataLayerRuntimeState::Activated);
		CurrentDataLayer = DataLayerToActivate;
	}
}

void UStateVisualHandlerComponent::HandleMPC(const FGameplayTag& NewState)
{
	// Landscape MPC 설정 
	UMaterialParameterCollectionInstance* LandscapeMPCInstance = GetWorld()->GetParameterCollectionInstance(LandscapeMPC);
	if (LandscapeMPCInstance)
	{
		// TODO : Parameter 이름을 리터럴로 넣지 않도록 해보기
		// Tint 수정	
		LandscapeMPCInstance->SetVectorParameterValue("Tint", VerseStateVisualMap->GetTint(NewState));
	}

}
