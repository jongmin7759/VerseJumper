// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/VJCharacterBase.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AVJCharacterBase::AVJCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AVJCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVJCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AVJCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AVJCharacterBase::EnterLadder(ULadderComponent* NewLadder)
{
	SetIsOnLadder(true);
	GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	GetCharacterMovement()->StopMovementImmediately();
	CurrentLadder = NewLadder;
}

void AVJCharacterBase::ExitLadder()
{
	SetIsOnLadder(false);
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	CurrentLadder = nullptr;
}

bool AVJCharacterBase::IsNearGround() const
{
	// 캡슐의 바닥을 기준으로 할 것
	const float BottomPos = GetActorLocation().Z -(GetCapsuleComponent()->GetScaledCapsuleHalfHeight()); 
	const FVector Start (GetActorLocation().X, GetActorLocation().Y, BottomPos);
	
	const FVector End = Start - FVector(0.f, 0.f, GroundDetectionDistance); // 캐릭터 아래 방향으로 50cm

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	const bool bHit = GetWorld()->LineTraceSingleByChannel(
		Hit,Start,End,ECC_Visibility,Params
	);

	return bHit;
}

