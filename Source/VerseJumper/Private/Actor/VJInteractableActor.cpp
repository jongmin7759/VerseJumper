// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/VJInteractableActor.h"

#include "Component/InteractionComponent.h"
#include "VerseJumper/VerseJumper.h"

AVJInteractableActor::AVJInteractableActor()
{
	PrimaryActorTick.bCanEverTick = false;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);
	// Custom Depth Stencil Value 설정 _ 매크로 정의는 VerseJumper.h에 있음
	InitHighlightColor();

	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractionComponent"));
}

void AVJInteractableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AVJInteractableActor::InitHighlightColor()
{
	if (StaticMesh)
	{
		StaticMesh->SetCustomDepthStencilValue(CUSTOM_DEPTH_DEFAULT);
	}
}

void AVJInteractableActor::HighlightActor()
{
	StaticMesh->SetRenderCustomDepth(true);
	bIsHighlighted = true;

	OnHighlight();
}

void AVJInteractableActor::UnHighlightActor()
{
	StaticMesh->SetRenderCustomDepth(false);
	bIsHighlighted = false;

	OnUnHighlight();
}

bool AVJInteractableActor::IsHighlighted() const
{
	return bIsHighlighted;
}

void AVJInteractableActor::SetHighlightColor(int32 Value)
{
	if (StaticMesh)
	{
		StaticMesh->SetCustomDepthStencilValue(Value);
	}

	OnHighlightColorChanged(Value);
}
