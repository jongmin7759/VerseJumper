
#include "Component/LadderComponent.h"

#include "Character/VJCharacterBase.h"
#include "Components/CapsuleComponent.h"

// Sets default values for this component's properties
ULadderComponent::ULadderComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void ULadderComponent::BeginPlay()
{
	Super::BeginPlay();

	// 중복 바인딩 방지
	if (OnComponentBeginOverlap.IsAlreadyBound(this,&ULadderComponent::EnterLadder))
	{
		OnComponentBeginOverlap.RemoveDynamic(this,&ULadderComponent::EnterLadder);
	}
	if (OnComponentEndOverlap.IsAlreadyBound(this,&ULadderComponent::ExitLadder))
	{
		OnComponentEndOverlap.RemoveDynamic(this,&ULadderComponent::ExitLadder);
	}
	OnComponentBeginOverlap.AddDynamic(this,&ULadderComponent::EnterLadder);
	OnComponentEndOverlap.AddDynamic(this,&ULadderComponent::ExitLadder);
	// TODO : 컴포넌트 숨기기
	// Debug 단계에서 사다리 보이게 하려고 켜둔 것
	SetHiddenInGame(false);
	SetLineThickness(5.f);
}

bool ULadderComponent::IsActorNearLadderTop(AActor* InActor) const
{
	if (InActor == nullptr) return false;

	const FVector LadderLocation = GetComponentLocation();
	const FVector LadderBounds = GetScaledBoxExtent();
	const float LadderTopPos = LadderLocation.Z + LadderBounds.Z - LadderTopOffset;
	
	const FVector ActorLocation = InActor->GetActorLocation();
	float BottomPos;
	// 캐릭터라면 캡슐 컴포넌트 사용
	if (ACharacter* Character = Cast<ACharacter>(InActor))
	{
		BottomPos = ActorLocation.Z - Character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	}
	// 캡슐 컴포넌트 없는 다른 클래스는 루트 컴포넌트의 바운드 사용하기 (부정확)
	else
	{
		const FBoxSphereBounds RootBounds = InActor->GetRootComponent()->Bounds;
		BottomPos = ActorLocation.Z - RootBounds.BoxExtent.Z;
	}
	
	// Ladder의 꼭대기보다 높게 올라갔으면 True
	return LadderTopPos <= BottomPos;
}



void ULadderComponent::EnterLadder(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AVJCharacterBase* VJCharacter = Cast<AVJCharacterBase>(OtherActor))
	{
		VJCharacter->EnterLadder(this);
	}
}

void ULadderComponent::ExitLadder(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (AVJCharacterBase* VJCharacter = Cast<AVJCharacterBase>(OtherActor))
	{
		VJCharacter->ExitLadder();
	}
}
