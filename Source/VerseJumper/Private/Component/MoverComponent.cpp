
#include "Component/MoverComponent.h"

UMoverComponent::UMoverComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UMoverComponent::BeginPlay()
{
	Super::BeginPlay();

	StartLocation = GetOwner()->GetActorLocation();
	
	if (!bTargetExplicitlySet)
	{
		// 임의의 위치 지정해둔 것 , 타겟 로케이션은 오너 액터에서 레벨 배치 보면서 직접 Set 호출해서 사용할 것
		TargetLocation = StartLocation + FVector(0, 0, 300.f);
		CurrentTargetLocation = bForward ? TargetLocation : StartLocation;
	}
	bIsMoving = bAutoStart;

}

void UMoverComponent::SwapDirection()
{
	bForward = !bForward;
	CurrentTargetLocation = bForward ? TargetLocation : StartLocation;
}


void UMoverComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

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
			//bIsMoving = false;
			// PauseTimer = PauseDuration;
			// GetWorld()->GetTimerManager().SetTimerForNextTick([this]() {
			// 	GetWorld()->GetTimerManager().SetTimer(
			// 		FTimerHandle(),
			// 		[this]() {
			// 			SwapDirection();
			// 			StartMoving();
			// 		},
			// 		PauseDuration,
			// 		false
			// 	);
			// });
			SwapDirection();
		}
		else
		{
			bIsMoving = false;
		}
	}
	else
	{
		Owner->SetActorLocation(CurrentLocation + Direction * MoveStep);
	}
}

void UMoverComponent::SetTargetLocation(FVector NewTargetLocation)
{
	TargetLocation = NewTargetLocation;
	bTargetExplicitlySet = true;
	CurrentTargetLocation = bForward ? TargetLocation : StartLocation;
}


void UMoverComponent::StartMoving()
{
	bIsMoving = true;
}

void UMoverComponent::StopMoving()
{
	bIsMoving = false;
}