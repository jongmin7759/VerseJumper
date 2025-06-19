// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/VerseStateResponsiveActor.h"

#include "GameplayTagContainer.h"
#include "DataAsset/VerseStateMeshSet.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystem/VerseStateSubsystem.h"

// Sets default values
AVerseStateResponsiveActor::AVerseStateResponsiveActor()
{
	PrimaryActorTick.bCanEverTick = false;

}

void AVerseStateResponsiveActor::SetCurrentStateEntry(FName Type, FGameplayTag State)
{
	CurrentStateEntry = VerseStateMeshSet->GetEntryFor(Type, State);
}

UStaticMesh* AVerseStateResponsiveActor::GetCurrentStaticMesh() const
{
	if (CurrentStateEntry)
	{
		return CurrentStateEntry->Mesh;
	}
	return nullptr;
}

float AVerseStateResponsiveActor::GetCurrentMeshScale() const
{
	if (CurrentStateEntry)
	{
		return CurrentStateEntry->MeshScale;
	}
	return 1.f;
}

void AVerseStateResponsiveActor::BeginPlay()
{
	Super::BeginPlay();

	// OnVerseStateChanged 바인딩
	UVerseStateSubsystem* VerseStateSubsystem = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UVerseStateSubsystem>();
	checkf(VerseStateSubsystem,TEXT("VerseStateSubsystem was NULL when tries to Bind OnVerseStateChanged"));
	VerseStateSubsystem->VerseStateChanged.AddUObject(this,&IVerseStateInterface::Internal_OnVerseStateChanged);

	// VerseStateMeshSet 정보가 없으면 메시 변환 어차피 안 됨
	checkf(VerseStateMeshSet,TEXT("VerseStateMeshSet was not initialized"));

	// 초기 StateSetting을 위해 초기화 호출 (액터 스폰 전에 초기 상태 전파가 이루어지기때문)
	InitializeCurrentState(VerseStateSubsystem->GetCurrentState());
}

void AVerseStateResponsiveActor::Internal_OnVerseStateChanged(const FGameplayTag& NewState)
{
	CurrentState = NewState;

	// DataAsset에 지정된 메시 엔트리 지정
	SetCurrentStateEntry(ActorType,NewState);
	// BP 이벤트 호출
	OnVerseStateChanged(NewState);
}

void AVerseStateResponsiveActor::InitializeCurrentState(const FGameplayTag& InitialState)
{
	Internal_OnVerseStateChanged(InitialState);
}

