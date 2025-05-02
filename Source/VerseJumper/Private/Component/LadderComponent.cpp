
#include "Component/LadderComponent.h"

#include "Character/VJCharacterBase.h"
#include "Components/ArrowComponent.h"

// Sets default values for this component's properties
ULadderComponent::ULadderComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>("ArrowComponent");
	ArrowComponent->SetupAttachment(this);
}

void ULadderComponent::BeginPlay()
{
	Super::BeginPlay();

	OnComponentBeginOverlap.AddDynamic(this,&ULadderComponent::EnterLadder);
	OnComponentEndOverlap.AddDynamic(this,&ULadderComponent::ExitLadder);
	
}

void ULadderComponent::EnterLadder(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// TODO : 사다리를 바라보고 있을 때만 탈 수 있도록 하기
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
