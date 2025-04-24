// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAsset/VerseStateVisualMap.h"

UDataLayerAsset* UVerseStateVisualMap::GetWorldPartitionLayer(FGameplayTag State) const
{
	if (!StateInfoMap.Contains(State)) return nullptr;
	
	return StateInfoMap.Find(State)->WorldPartitionLayer;
}

FColor UVerseStateVisualMap::GetTint(FGameplayTag State) const
{
	if (!StateInfoMap.Contains(State)) return FColor::Black;
	
	return StateInfoMap.Find(State)->Tint;
}
