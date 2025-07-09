
#include "Component/MoverComponent.h"

UMoverComponent::UMoverComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UMoverComponent::BeginPlay()
{
	Super::BeginPlay();

	Init();

}

void UMoverComponent::Init()
{
	// 완전 초기 상태가 아니라 다시 온 되는 상황일 때는 오프된 기간의 움직임 보정해주기
	// Mover 동작 중이었을 경우에만 시뮬레이션하면 됨
	if (bIsInitialized)
	{
		if (bIsActivated)
		{
			RestoreMovementFromElapsedTime(GetWorld()->GetTimeSeconds() - StartTime);
		}
		// 보정 필요없을 경우에는 Start 덮어씌워지지 않도록 얼리 리턴
		return;
	} 
	bIsInitialized = true;

	StartTransform = GetOwner()->GetTransform();
	StartLocation = GetOwner()->GetActorLocation();
	if (!bTargetExplicitlySet)
	{
		// 액터에서 오버라이딩하지 않는다면 컴포넌트에 지정한 상대위치로 타겟 위치 설정 
		UpdateTargetLocationFromRelative();
	}
	// 방향에 따라 타겟 위치 설정
	UpdateCurrentTargetLocation();
	// 초기 설정에 따라 이동 세팅
	if (bAutoStart) StartMoving();
}

void UMoverComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	MoveActor(DeltaTime);
}

void UMoverComponent::ResetPosition()
{
	AActor* Owner = GetOwner();
	if (Owner == nullptr || GetWorld() == nullptr) return;

	// 시작 위치로 초기화 , 방향 초기화
	Owner->SetActorLocation(StartLocation);
	bForward = true;
	UpdateStartTime();
}

void UMoverComponent::RestoreMovementFromElapsedTime(float ElapsedTime)
{
	AActor* Owner = GetOwner();
	if (Owner == nullptr) return;
	
	const float Distance = FVector::Dist(StartLocation, TargetLocation);
	const float MoveTime = Distance / Speed;
	const float TotalCycleTime = MoveTime * 2.0f + PauseDuration * 2.0f;

	
	float T = TotalCycleTime > 0 ? FMath::Fmod(ElapsedTime, TotalCycleTime) : 0.0f;
	
	TWeakObjectPtr<UMoverComponent> WeakThis(this);
	
	if (T < MoveTime) // Moving forward
	{
		float Alpha = T / MoveTime;
		bForward = true;
		StartMoving();
		Owner->SetActorLocation(FMath::Lerp(StartLocation, TargetLocation, Alpha));
	}
	else if (T < MoveTime + PauseDuration) // Paused at Target
	{
		PauseMoving();
		Owner->SetActorLocation(TargetLocation);

		float RemainingPause = PauseDuration - (T - MoveTime);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [WeakThis]()
		{
			if (WeakThis.IsValid())
			{
				WeakThis->bForward = false;
			   WeakThis->StartMoving();
			}
		}, RemainingPause, false);
	}
	else if (T < MoveTime * 2.0f + PauseDuration) // Moving backward
	{
		float Alpha = (T - MoveTime - PauseDuration) / MoveTime;
		bForward = false;
		StartMoving();
		Owner->SetActorLocation(FMath::Lerp(TargetLocation, StartLocation, Alpha));
	}
	else // Paused at Start
	{
		bIsMoving = false;
		Owner->SetActorLocation(StartLocation);

		float RemainingPause = PauseDuration - (T - (MoveTime * 2.0f + PauseDuration));
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [WeakThis]()
		{
			if (WeakThis.IsValid())
			{
				WeakThis->bForward = true;
				WeakThis->StartMoving();
			}
		}, RemainingPause, false);
	}
}

void UMoverComponent::UpdateStartTime()
{
	AActor* Owner = GetOwner();
	if (Owner == nullptr || GetWorld() == nullptr) return;
	
	StartTime = GetWorld()->GetTimeSeconds();

	const float Distance = FVector::Dist(StartLocation, TargetLocation);
	const float MoveTime = Distance / Speed;
	float Alpha = FVector::Dist(StartLocation, Owner->GetActorLocation()) / Distance;
	
	float ElapsedTime = 0.f;
	// 순방향
	if (bForward)
	{
		ElapsedTime = MoveTime * Alpha;
	}
	else
	{
		ElapsedTime = PauseDuration + MoveTime*(2.0f - Alpha);
	}

	StartTime -= ElapsedTime;
	if (StartTime < 0.f) StartTime = 0.f;
}


void UMoverComponent::UpdateTargetLocationFromRelative()
{ 
	TargetLocation = StartTransform.TransformPosition(RelativeTargetLocation);
}

void UMoverComponent::UpdateCurrentTargetLocation()
{
	CurrentTargetLocation = bForward ? TargetLocation : StartLocation;
}

void UMoverComponent::SwapDirection()
{
	bForward = !bForward;
	UpdateCurrentTargetLocation();
}

void UMoverComponent::MoveActor(float DeltaTime)
{
	if (!bIsMoving) return;
	AActor* Owner = GetOwner();
	if (!Owner) return;
	
	FVector CurrentLocation = Owner->GetActorLocation();
	FVector Direction = (CurrentTargetLocation - CurrentLocation).GetSafeNormal();
	float DistanceToTarget = FVector::Dist(CurrentLocation, CurrentTargetLocation);

	float MoveStep = Speed * DeltaTime;
	if (MoveStep >= DistanceToTarget)
	{
		Owner->SetActorLocation(CurrentTargetLocation);

		if (bAutoPingPong)
		{
			// SetTimer에서 0 이하면 타이머 Clear하므로 정지하지 않는 경우 바로 방향 전환 후 리턴시키기
			if (PauseDuration <= 0.f)
			{
				SwapDirection();
				return;
			}
			
			PauseMoving();
			TWeakObjectPtr<UMoverComponent> WeakThis(this);
			GetWorld()->GetTimerManager().SetTimerForNextTick(
				[WeakThis]()
				{
					if (WeakThis.IsValid())
					{
						 WeakThis->GetWorld()->GetTimerManager().SetTimer(WeakThis->TimerHandle,
							 [WeakThis]()
							 {
								 if (WeakThis.IsValid() && WeakThis->bIsActivated)
								 {
									 WeakThis->SwapDirection();
									 WeakThis->ResumeMoving();
								 }
							 },WeakThis->PauseDuration,false);
					}
				});
		}
		else
		{
			PauseMoving();
		}
	}
	else
	{
		Owner->SetActorLocation(CurrentLocation + Direction * MoveStep);
	}
}

void UMoverComponent::SetRelativeTargetLocation(FVector NewTargetLocation)
{
	RelativeTargetLocation = NewTargetLocation;
	UpdateTargetLocationFromRelative();
	UpdateCurrentTargetLocation();
}


void UMoverComponent::OverrideTargetLocation(FVector NewTargetLocation)
{
	TargetLocation = NewTargetLocation;
	bTargetExplicitlySet = true;
	UpdateCurrentTargetLocation();
}


void UMoverComponent::StartMoving()
{
	bIsMoving = true;
	bIsActivated = true;
	UpdateStartTime();
}

void UMoverComponent::StopMoving()
{
	bIsMoving = false;
	bIsActivated = false;
	TimerHandle.Invalidate();
}

void UMoverComponent::PauseMoving()
{
	bIsMoving = false;
}

void UMoverComponent::ResumeMoving()
{
	bIsMoving = true;
}

