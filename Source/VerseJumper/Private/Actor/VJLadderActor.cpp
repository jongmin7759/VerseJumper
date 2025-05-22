// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/VJLadderActor.h"


#include "Component/LadderComponent.h"

#include "Character/VJCharacterBase.h"
#include "Components/CapsuleComponent.h"

// Sets default values for this component's properties
AVJLadderActor::AVJLadderActor()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>("CollisionBox");
	RootComponent = CollisionBox;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMesh->SetupAttachment(RootComponent);
	// 사다리 메시는 보여주기만
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}


void AVJLadderActor::BeginPlay()
{
	Super::BeginPlay();

	// 중복 바인딩 방지
	if (CollisionBox->OnComponentBeginOverlap.IsAlreadyBound(this,&AVJLadderActor::EnterLadder))
	{
		CollisionBox->OnComponentBeginOverlap.RemoveDynamic(this,&AVJLadderActor::EnterLadder);
	}
	if (CollisionBox->OnComponentEndOverlap.IsAlreadyBound(this,&AVJLadderActor::ExitLadder))
	{
		CollisionBox->OnComponentEndOverlap.RemoveDynamic(this,&AVJLadderActor::ExitLadder);
	}
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this,&AVJLadderActor::EnterLadder);
	CollisionBox->OnComponentEndOverlap.AddDynamic(this,&AVJLadderActor::ExitLadder);
}

bool AVJLadderActor::IsActorNearLadderTop(AActor* InActor) const
{
	if (InActor == nullptr) return false;

	const FVector LadderLocation = GetActorLocation();
	const FVector LadderBounds = CollisionBox->GetScaledBoxExtent();
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



void AVJLadderActor::EnterLadder(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AVJCharacterBase* VJCharacter = Cast<AVJCharacterBase>(OtherActor))
	{
		VJCharacter->EnterLadder(this);
	}
}

void AVJLadderActor::ExitLadder(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (AVJCharacterBase* VJCharacter = Cast<AVJCharacterBase>(OtherActor))
	{
		VJCharacter->ExitLadder();
	}
}

void AVJLadderActor::Internal_OnVerseStateChanged(const FGameplayTag& NewState)
{
	Super::Internal_OnVerseStateChanged(NewState);
	
	UStaticMesh* NewMesh = GetCurrentStaticMesh();
	if (NewMesh == nullptr) return;
	
	// 메시를 변경하고 콜리전 박스 크기에 맞춰 사이즈 보정까지 해준다.
	StaticMesh->SetStaticMesh(NewMesh);
	const FVector BoxExtent = CollisionBox->GetScaledBoxExtent();
	const FVector MeshBounds = GetCurrentStaticMesh()->GetBounds().BoxExtent;
	const FVector ScaleToFit = BoxExtent / MeshBounds;
	StaticMesh->SetWorldScale3D(ScaleToFit);
}
