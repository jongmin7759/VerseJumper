// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/FootstepComponent.h"

#include "Character/VJCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "DataAsset/SurfaceSoundMap.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"


UFootstepComponent::UFootstepComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UFootstepComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerVJCharacter = Cast<AVJCharacterBase>(GetOwner());
}

void UFootstepComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	const UCharacterMovementComponent* CharacterMovementComponent = OwnerVJCharacter->GetCharacterMovement();
	if (OwnerVJCharacter == nullptr || CharacterMovementComponent == nullptr) return;

	const bool bIsOnLadder = OwnerVJCharacter->GetIsOnLadder();
	const bool bIsMoving = OwnerVJCharacter->GetVelocity().Length() > 0.f;
	const bool bIsFalling = CharacterMovementComponent->IsFalling();
	
	if (bIsMoving == false || bIsFalling == true)
	{
		TimeSinceLastStep = 0.f;
		return;
	}
	
	TimeSinceLastStep += DeltaTime;
	if (TimeSinceLastStep >= StepInterval)
	{
		PlayFootstepSound(bIsOnLadder);
		TimeSinceLastStep -= StepInterval;
	}
}

void UFootstepComponent::PlayFootstepSound(const bool bIsOnLadder)
{
	if (OwnerVJCharacter == nullptr) return;

	// Sound 받아오기
	USoundBase* FootstepSound = GetFootstepSound(bIsOnLadder);
	if (FootstepSound == nullptr) return;

	// 캡슐의 바닥을 재생 위치로
	const float BottomPos = OwnerVJCharacter->GetActorLocation().Z -(OwnerVJCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight()); 
	const FVector BottomLocation (OwnerVJCharacter->GetActorLocation().X, OwnerVJCharacter->GetActorLocation().Y, BottomPos);
	UGameplayStatics::PlaySoundAtLocation(this, FootstepSound, BottomLocation);
}

USoundBase* UFootstepComponent::GetFootstepSound(const bool bIsOnLadder) const
{
	if (OwnerVJCharacter == nullptr || SurfaceSoundMap == nullptr) return nullptr;

	const ULadderComponent* CurrentLadder = OwnerVJCharacter->GetCurrentLadder();
	if (bIsOnLadder && CurrentLadder)
	{
		EPhysicalSurface SurfaceType = CurrentLadder->GetLadderSurface();
			
		return SurfaceSoundMap->GetFootstepSound(SurfaceType);
	}
	// Ground라면 라인트레이스를 통해 바닥의 피지컬 머티리얼을 받아와 판단
	else
	{
		const float BottomPos = OwnerVJCharacter->GetActorLocation().Z -(OwnerVJCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight()); 
		const FVector Start (OwnerVJCharacter->GetActorLocation().X, OwnerVJCharacter->GetActorLocation().Y, BottomPos);
		const FVector End = Start - FVector(0.f, 0.f, 50.f); // 캐릭터 아래 방향으로 50cm
		
		FHitResult HitResult;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(OwnerVJCharacter);

		if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params))
		{
			EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(HitResult.PhysMaterial.Get());
			
			return SurfaceSoundMap->GetFootstepSound(SurfaceType);
		}
		
	}
	return nullptr;
}
