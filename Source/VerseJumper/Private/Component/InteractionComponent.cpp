// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/InteractionComponent.h"

// Sets default values for this component's properties
UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}


void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

}

bool UInteractionComponent::TryInteract()
{
	if (bCanInteract)
	{
		OnInteractSuccess.Broadcast();
		return true;
	}
	else
	{
		OnInteractFailed.Broadcast();
		return false;
	}
}

FICMetaData UInteractionComponent::GetMetaData() const
{
	return FICMetaData(bCanInteract,ActorName,ActionName);
}
