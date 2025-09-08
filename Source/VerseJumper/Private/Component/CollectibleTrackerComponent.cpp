// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/CollectibleTrackerComponent.h"

#include "SteamToggle.h"
#include "Game/VJGameModeBase.h"

UCollectibleTrackerComponent::UCollectibleTrackerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UCollectibleTrackerComponent::InitCollectibleTracker(const TSet<FName>& IDs,int32 NumCollected)
{
	SetCollectedIDs(IDs);
	SetTotalCollectedNum(NumCollected);
	
	OnCollectibleUpdated.Broadcast(TotalCollected,true);


	// steam test
	if (SteamToggle::IsUseSteam() && SteamApps())
	{
		SteamUserStats()->ClearAchievement("Test1");
		SteamUserStats()->StoreStats();
	}
}

void UCollectibleTrackerComponent::RegisterCollectible(FName CollectibleID)
{
	if (!CollectedIDs.Contains(CollectibleID))
	{
		CollectedIDs.Add(CollectibleID);
		TotalCollected++;
		OnCollectibleUpdated.Broadcast(TotalCollected,false);
		// 도전과제 처리
		HandleAchievement();
	}
}

void UCollectibleTrackerComponent::HandleAchievement() const
{
	if (AVJGameModeBase* VJGameMode = Cast<AVJGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		switch (TotalCollected)
		{
			case 1 :
				VJGameMode->UnlockAchievement("Bagel_1");
				break;
			case 6 :
				VJGameMode->UnlockAchievement("Bagel_6");
				break;
			case 12 :
				VJGameMode->UnlockAchievement("Bagel_12");
				break;
			case 24 :
				VJGameMode->UnlockAchievement("Bagel_24");
				break;
			default: break;
		}
	}
}
