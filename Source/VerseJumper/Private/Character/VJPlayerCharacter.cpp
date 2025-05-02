// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/VJPlayerCharacter.h"

#include "Components/SphereComponent.h"
#include "Kismet/KismetSystemLibrary.h"

AVJPlayerCharacter::AVJPlayerCharacter()
{
	JumpBlocker = CreateDefaultSubobject<USphereComponent>("JumpBlocker");
	JumpBlocker->SetupAttachment(RootComponent);
	// 시작은 콜리전 꺼져있는 상태로
	JumpBlocker->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AVJPlayerCharacter::HandleMovementInput(const FVector2D& Input)
{
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	const FVector UpDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Z);

	
	// 사다리에 탑승한 상태라면 위아래로만 이동
	if (bIsOnLadder)
	{
		AddMovementInput(UpDirection,Input.Y);

		// 사다리 내리기 (내려가는 중이면서 바닥에 근접했으면 탈출)
		if (Input.Y < 0.f && IsNearGround())
		{
			ExitLadder();
		}
	}
	else
	{
		AddMovementInput(ForwardDirection,Input.Y);
		AddMovementInput(RightDirection,Input.X);
	}
}

void AVJPlayerCharacter::HandleLookInput(const FVector2D& Input)
{
	// 사다리에 탄 상태라면 각도 제한하기
	if (bIsOnLadder && CurrentLadder)
	{
		const FVector BaseForwardVector = -CurrentLadder->GetForwardVector();
		const float BaseYaw = BaseForwardVector.Rotation().Yaw;
		FRotator ControlRot = Controller->GetControlRotation();
		const float RawYaw = ControlRot.Yaw;
		const float NormalizedYaw = FMath::UnwindDegrees(RawYaw);
		ControlRot.Yaw = NormalizedYaw;
		
		//Min
		const float Min = BaseYaw - 90.f;
		// Max
		const float Max = BaseYaw + 90.f;
		const float NewYaw = FMath::Clamp(ControlRot.Yaw + Input.X, Min, Max);
		const float NewPitch = ControlRot.Pitch - Input.Y;
		
		Controller->SetControlRotation(FRotator(NewPitch, NewYaw, 0.f));
	}
	else
	{
		AddControllerYawInput(Input.X);
		AddControllerPitchInput(Input.Y);
	}
}

bool AVJPlayerCharacter::CanJumpInternal_Implementation() const
{
	// Character에서 Can Jump True일 때만 추가 검사
	if (Super::CanJumpInternal_Implementation())
	{
		// JumpBlocker에 이미 겹쳐있는 경우 제대로 점프가 막히지 않는 경우가 있어 이런 경우 아예 점프가 안 되도록 만들기
		FHitResult HitResult;
		const FVector Start = JumpBlocker->GetComponentLocation();
		const FVector End = Start + 0.1f;
		const float Radius = JumpBlocker->GetScaledSphereRadius();

		TArray<AActor*> IgnoredActors;
		//IgnoredActors.Add(this);

		const bool bHit = UKismetSystemLibrary::SphereTraceSingle(
			this,Start,End,Radius,UEngineTypes::ConvertToTraceType(ECC_WorldStatic),
			false,IgnoredActors,EDrawDebugTrace::None,HitResult,true);
		// 충돌 결과가 감지되면 (bHit == True) CanJump == False여야하므로 반대값 리턴
		return !bHit;
	}
	else return false;
}
