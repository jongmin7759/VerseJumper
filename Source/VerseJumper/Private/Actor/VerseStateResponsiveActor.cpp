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


// Called when the game starts or when spawned
void AVerseStateResponsiveActor::BeginPlay()
{
	Super::BeginPlay();

	// OnVerseStateChanged 바인딩
	UVerseStateSubsystem* VerseStateSubsystem = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UVerseStateSubsystem>();
	checkf(VerseStateSubsystem,TEXT("VerseStateSubsystem was NULL when tries to Bind OnVerseStateChanged"));
	VerseStateSubsystem->VerseStateChanged.AddUObject(this,&AVerseStateResponsiveActor::Internal_OnVerseStateChanged);

	// VerseStateMeshSet 정보가 없으면 메시 변환 어차피 안 됨
	checkf(VerseStateMeshSet,TEXT("VerseStateMeshSet was not initialized"));
}

void AVerseStateResponsiveActor::Internal_OnVerseStateChanged(const FGameplayTag& NewState)
{
	CurrentState = NewState;

	// DataAsset에 지정된 스태틱 메시 지정
	SetCurrentStaticMesh(VerseStateMeshSet->GetMeshFor(ActorType,NewState));
	// BP 이벤트 호출
	OnVerseStateChanged(NewState);
}

