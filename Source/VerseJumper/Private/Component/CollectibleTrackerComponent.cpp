// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/CollectibleTrackerComponent.h"

UCollectibleTrackerComponent::UCollectibleTrackerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UCollectibleTrackerComponent::InitCollectibleTracker()
{
	OnCollectibleUpdated.Broadcast(TotalCollected,true);
}

void UCollectibleTrackerComponent::RegisterCollectible(FName CollectibleID)
{
	if (!CollectedIDs.Contains(CollectibleID))
	{
		CollectedIDs.Add(CollectibleID);
		TotalCollected++;
		OnCollectibleUpdated.Broadcast(TotalCollected,false);
	}
}


void UCollectibleTrackerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}
