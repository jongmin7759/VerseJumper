// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/VJInteractableActor.h"

#include "VerseJumper/VerseJumper.h"

AVJInteractableActor::AVJInteractableActor()
{
	PrimaryActorTick.bCanEverTick = false;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);
	// Custom Depth Stencil Value 설정 _ 매크로 정의는 VerseJumper.h에 있음
	StaticMesh->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void AVJInteractableActor::BeginPlay()
{
	Super::BeginPlay();
	
}
void AVJInteractableActor::HighlightActor()
{
	StaticMesh->SetRenderCustomDepth(true);
	bIsHighlighted = true;
}

void AVJInteractableActor::UnHighlightActor()
{
	StaticMesh->SetRenderCustomDepth(false);
	bIsHighlighted = false;
}

bool AVJInteractableActor::IsHighlighted() const
{
	return bIsHighlighted;
}
