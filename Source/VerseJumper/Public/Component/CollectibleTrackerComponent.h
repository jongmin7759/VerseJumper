// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CollectibleTrackerComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnCollectibleUpdatedSignature,int32,bool);
///
///	수집품 목록 관리 (세이브/로드 , ID 저장 , 카운팅)
///
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VERSEJUMPER_API UCollectibleTrackerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCollectibleTrackerComponent();
	FOnCollectibleUpdatedSignature OnCollectibleUpdated;
	// 로드할 때는 도전과제 갱신 안 하도록
	void InitCollectibleTracker(const TSet<FName>& IDs, int32 NumCollected);
	UFUNCTION(BlueprintCallable)
	void RegisterCollectible(FName CollectibleID);
	UFUNCTION(BlueprintCallable)
	int32 GetTotalCollectedNum() const {return TotalCollected;}
	void SetTotalCollectedNum(int32 NumCollected) {TotalCollected = NumCollected;}
	const TSet<FName>& GetCollectedIDs() const {return CollectedIDs;}
	void SetCollectedIDs(const TSet<FName>& IDs) {CollectedIDs = IDs;}

private:
	TSet<FName> CollectedIDs;
	int32 TotalCollected = 0;
	
	void HandleAchievement() const;	
};
