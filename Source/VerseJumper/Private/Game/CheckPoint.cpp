// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/CheckPoint.h"

#include "Character/VJPlayerCharacter.h"
#include "Component/LifeCycleSaveComponent.h"
#include "Components/SphereComponent.h"
#include "Game/VJGameModeBase.h"
#include "Kismet/GameplayStatics.h"

ACheckPoint::ACheckPoint(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	
	CheckpointMesh = CreateDefaultSubobject<UStaticMeshComponent>("CheckpointMesh");
	CheckpointMesh->SetupAttachment(GetRootComponent());
	CheckpointMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CheckpointMesh->SetCollisionResponseToAllChannels(ECR_Block);

	SetHighlightColor(CustomDepthStencilOverride);
	CheckpointMesh->MarkRenderStateDirty();

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	Sphere->SetupAttachment(CheckpointMesh);
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	SaveComponent = CreateDefaultSubobject<ULifeCycleSaveComponent>("SaveComponent");
	SaveComponent->OnLoaded.AddDynamic(this,&ACheckPoint::OnLoaded);
}

void ACheckPoint::BeginPlay()
{
	Super::BeginPlay();
	if (!bReached)
	{
		Sphere->OnComponentBeginOverlap.AddDynamic(this, &ACheckPoint::OnSphereOverlap);
	}
}

void ACheckPoint::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AVJPlayerCharacter* PC = Cast<AVJPlayerCharacter>(OtherActor))
	{
		if (AVJGameModeBase* VJGameMode = Cast<AVJGameModeBase>(UGameplayStatics::GetGameMode(this)))
		{
			bReached = true;
			SaveComponent->SaveOwnerActor();
			PC->SavePlayerProgress(PlayerStartTag);
			VJGameMode->SaveGameSlot();
		}
		
		CheckPointReached();
	}
}


void ACheckPoint::OnLoaded()
{
	if (bReached)
	{
		CheckPointReached(true);
	}
}

void ACheckPoint::CheckPointReached_Implementation(bool ShouldSkipEffect)
{
	// 중복 안 되도록 콜리전 끄기
	Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);	
}

void ACheckPoint::HighlightActor()
{
	if (!bReached)
	{
		CheckpointMesh->SetRenderCustomDepth(true);
		bIsHighlighted = true;
	}
}

void ACheckPoint::UnHighlightActor()
{
	CheckpointMesh->SetRenderCustomDepth(false);
	bIsHighlighted = false;
}

bool ACheckPoint::IsHighlighted() const
{
	return bIsHighlighted;
}

void ACheckPoint::SetHighlightColor(int32 Value)
{
	if (CheckpointMesh)
	{
		CheckpointMesh->SetCustomDepthStencilValue(Value);
	}
}
