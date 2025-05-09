// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "VerseStateVisualMap.generated.h"

USTRUCT(BlueprintType)
struct FVerseStateVisualInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UDataLayerAsset* WorldPartitionLayer = nullptr;

	UPROPERTY(EditAnywhere)
	FColor Tint = FColor::Black;
};
/**
 * 
 */
UCLASS()
class VERSEJUMPER_API UVerseStateVisualMap : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	TMap<FGameplayTag,FVerseStateVisualInfo> StateInfoMap;

	// State에 대응하는 데이터레이어 반환
	UDataLayerAsset* GetWorldPartitionLayer(FGameplayTag State) const;
	// Tint값 반환
	FColor GetTint(FGameplayTag State) const;
};
