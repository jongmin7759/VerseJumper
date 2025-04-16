// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/VerseStateResponsiveActor.h"

#include "GameplayTagContainer.h"
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
}

void AVerseStateResponsiveActor::Internal_OnVerseStateChanged(const FGameplayTag& NewState)
{
	UE_LOG(LogTemp, Display, TEXT("NewState : %s "),*NewState.ToString());
	CurrentState = NewState;
	// BP 이벤트 호출
	OnVerseStateChanged(NewState);
}

