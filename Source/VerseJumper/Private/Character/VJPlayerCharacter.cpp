// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/VJPlayerCharacter.h"

#include "Actor/VJLadderActor.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
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
	
	if (bIsOnLadder && CurrentLadder)
	{
		// 사다리 내리기 (내려가는 중이면서 바닥에 근접했으면 탈출)
		if (Input.Y < 0.f && IsNearGround())
		{
			ExitLadder();
		}
		// 올라가는 중이면서 꼭대기에 올라왔으면 탈출 가능하도록 앞으로도 이동
		else if (Input.Y > 0.f && CurrentLadder->IsActorNearLadderTop(this))
		{
			AddMovementInput(ForwardDirection,Input.Y);
			return;
		}
		// 사다리 탈출하는 경우가 아니라면 앞뒤 대신 위아래 이동
		AddMovementInput(UpDirection,Input.Y);
	}
	else
	{
		AddMovementInput(ForwardDirection,Input.Y);
		AddMovementInput(RightDirection,Input.X);
	}
}

void AVJPlayerCharacter::HandleLookInput(const FVector2D& Input)
{
	AddControllerYawInput(Input.X);
	AddControllerPitchInput(Input.Y);
}

void AVJPlayerCharacter::OnJumped_Implementation()
{
	OnJumpBegin.Broadcast();
	PlaySFX(JumpSound);
}

void AVJPlayerCharacter::EnterLadder(AVJLadderActor* NewLadder)
{
	Super::EnterLadder(NewLadder);
	// 사다리에 들어가도 점프는 종료임
	OnJumpEnd.Broadcast();
}

void AVJPlayerCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	PlaySFX(LandingSound);
	// 착지하면 점프 종료
	OnJumpEnd.Broadcast();
}

void AVJPlayerCharacter::PressModifier()
{
	OnModifierPressed.Broadcast();
	bIsModifierPressed = true;
}

void AVJPlayerCharacter::ReleaseModifier()
{
	OnModifierReleased.Broadcast();
	bIsModifierPressed = false;
}

bool AVJPlayerCharacter::CanJumpInternal_Implementation() const
{
	// Modifier 눌려있으면 점프 X
	if (bIsModifierPressed) return false;
	
	// Character에서 Can Jump True일 때만 추가 검사
	if (Super::CanJumpInternal_Implementation())
	{
		// JumpBlocker에 이미 겹쳐있는 경우 제대로 점프가 막히지 않는 경우가 있어 이런 경우 아예 점프가 안 되도록 만들기
		FHitResult HitResult;
		const FVector Start = JumpBlocker->GetComponentLocation();
		const FVector End = Start + 0.1f;
		const float Radius = JumpBlocker->GetScaledSphereRadius();
	
		TArray<AActor*> IgnoredActors;
	
		const bool bHit = UKismetSystemLibrary::SphereTraceSingle(
			this,Start,End,Radius,UEngineTypes::ConvertToTraceType(ECC_WorldStatic),
			false,IgnoredActors,EDrawDebugTrace::None,HitResult,true);
		// 충돌 결과가 감지되면 (bHit == True) CanJump == False여야하므로 반대값 리턴
		return !bHit;
	}
	else return false;
}

void AVJPlayerCharacter::Internal_OnVerseStateChanged(const FGameplayTag& NewState)
{
	//Super::Internal_OnVerseStateChanged(NewState);

	PlaySFX(VerseJumpSound);
}

void AVJPlayerCharacter::PlaySFX(USoundBase* SoundBase) const
{
	if (SoundBase == nullptr)
	{
		UE_LOG(LogTemp,Warning,TEXT("AVJPlayerCharacter::PlaySFX() : Invalid Sound"));
		return;
	}
	UGameplayStatics::PlaySoundAtLocation(this, SoundBase, GetActorLocation());
}
