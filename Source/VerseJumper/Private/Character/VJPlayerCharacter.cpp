// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/VJPlayerCharacter.h"

#include "Actor/VJLadderActor.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "Interface/HighlightInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "VerseJumper/VerseJumper.h"

AVJPlayerCharacter::AVJPlayerCharacter()
{
	JumpBlocker = CreateDefaultSubobject<USphereComponent>("JumpBlocker");
	JumpBlocker->SetupAttachment(RootComponent);
	// 시작은 콜리전 꺼져있는 상태로
	JumpBlocker->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 콜리전 채널 설정
	HighlightInvokerSphere = CreateDefaultSubobject<USphereComponent>("HighlightInvokerSphere");
	HighlightInvokerSphere->SetupAttachment(RootComponent);
	HighlightInvokerSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	HighlightInvokerSphere->SetCollisionObjectType(ECC_Highlight);
	HighlightInvokerSphere->SetCollisionResponseToAllChannels(ECR_Overlap);
}

void AVJPlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	InteractionTrace();
}

void AVJPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	HighlightInvokerSphere->OnComponentBeginOverlap.AddDynamic(this,&AVJPlayerCharacter::AddHighlightCandidate);
	HighlightInvokerSphere->OnComponentEndOverlap.AddDynamic(this,&AVJPlayerCharacter::RemoveHighlightCandidate);

	if (const UCameraComponent* PlayerCamera = GetComponentByClass<UCameraComponent>())
	{
		// Spring Arm이 있어서 단순 상대 위치로는 오차 생겨서 직접 계산
		CameraHeightOffset = PlayerCamera->GetComponentLocation().Z - GetActorLocation().Z;
	}
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

bool AVJPlayerCharacter::IsInViewAngle(AActor* Target,float Angle) const
{
	FVector CameraLocation = GetActorLocation();
	FVector CameraForward = GetActorForwardVector();
	
	// 카메라 컴포넌트 있으면 카메라 위치로
	const UCameraComponent* PlayerCamera = GetComponentByClass<UCameraComponent>();
	if (PlayerCamera)
	{
		CameraLocation = PlayerCamera->GetComponentLocation();
		CameraForward = PlayerCamera->GetForwardVector();
	}

	FVector TargetLocation = Target->GetActorLocation();

	// 시야 체크
	FVector ToTarget = (TargetLocation - CameraLocation).GetSafeNormal();
	float Dot = FVector::DotProduct(CameraForward, ToTarget);

	// 시야각 / 2의 코사인과 비교 (ex. 60도 시야 → cos(30도))
	float CosHalfFOV = FMath::Cos(FMath::DegreesToRadians(Angle / 2.f));

	return Dot >= CosHalfFOV;
}
void AVJPlayerCharacter::GetFilteredHighlightCandidates(TSet<TWeakObjectPtr<AActor>>& OutCandidates) const
{
	OutCandidates.Reset();
	OutCandidates.Reserve(HighlightCandidates.Num());

	// 시야각 체크해서 그 안에 들어있는 애들만 넘겨줌
	for (const TWeakObjectPtr<AActor>& Candidate : HighlightCandidates)
	{
		if (!Candidate.IsValid()) continue;

		if (IsInViewAngle(Candidate.Get(),HighlightAngle))
		{
			OutCandidates.Add(Candidate);
		}
	}
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

void AVJPlayerCharacter::AddHighlightCandidate(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->Implements<UHighlightInterface>())
	{
		HighlightCandidates.Add(OtherActor);
	}
}

void AVJPlayerCharacter::RemoveHighlightCandidate(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor->Implements<UHighlightInterface>())
	{
		HighlightCandidates.Remove(OtherActor);
	}
}

void AVJPlayerCharacter::InteractionTrace()
{
	//TODO : 거리만큼 라인트레이스
	UWorld* World = GetWorld();
	if (!World) return;
	FHitResult HitResult;
	// 카메라 높이 보정
	const FVector Start = GetActorLocation() + FVector(0, 0, CameraHeightOffset);
	const FVector ForwardNormal = GetControlRotation().Vector().GetSafeNormal();
	const FVector End = Start + (ForwardNormal * InteractionDistance);

	FCollisionQueryParams Params;
	Params.bTraceComplex = false;
	Params.AddIgnoredActor(this);

	// Hit 성공
	if (World->LineTraceSingleByChannel(HitResult,Start, End, ECC_Interaction, Params))
	{
		CurrentInteractingActor = HitResult.GetActor();
	}
	else
	{
		CurrentInteractingActor = nullptr;
	}
	
	///1. 현재 액터와 이전 액터 구분해서
	///1-1. 현재 액터 == 이전 액터
	if (CurrentInteractingActor == LastInteractingActor)
	{
		// 컨트롤러에 알릴 필요 없음
	}
	///1-2. 현재 액터 != 이전 액터
	else
	{
		LastInteractingActor = CurrentInteractingActor;
		if (CurrentInteractingActor.IsValid())
		{
			OnActorDetected.Broadcast(CurrentInteractingActor.Get());
		}
		else
		{
			OnClearedInteractionActor.Broadcast();
		}
	}
}

