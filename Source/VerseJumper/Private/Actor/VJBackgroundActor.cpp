// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/VJBackgroundActor.h"

#include "Components/InstancedStaticMeshComponent.h"

AVJBackgroundActor::AVJBackgroundActor()
{
	InstancedStaticMeshComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>("InstancedStaticMesh");
	InstancedStaticMeshComponent->SetupAttachment(RootComponent);
	InstancedStaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AVJBackgroundActor::Internal_OnVerseStateChanged(const FGameplayTag& NewState)
{
	Super::Internal_OnVerseStateChanged(NewState);

	InstancedStaticMeshComponent->SetStaticMesh(GetCurrentStaticMesh());
}
