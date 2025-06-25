// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/VerseStateSubsystem.h"

void UVerseStateSubsystem::SetNewState(const FGameplayTag& NewState)
{
	// 현재 상태와 같은 상태라면 얼리 리턴
	if (NewState == CurrentState) return;
	
	const FGameplayTag VerseState = FGameplayTag::RequestGameplayTag("VerseState");
	if (NewState.MatchesTag(VerseState))
	{
		CurrentState = NewState;
		// 브로드캐스팅
		VerseStateChanged.Broadcast(NewState);
	}
	else
	{
		UE_LOG(LogTemp,Error,TEXT("Tries To Set Wrong VerseState!!"));
	}
}
