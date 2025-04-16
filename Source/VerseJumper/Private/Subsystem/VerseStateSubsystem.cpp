// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/VerseStateSubsystem.h"

void UVerseStateSubsystem::SetNewState(const FGameplayTag NewState)
{
	// 현재 상태와 같으면 얼리 리턴
	if (CurrentState == NewState) return;
	
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

FGameplayTag UVerseStateSubsystem::GetPrevAvailableState()
{
	int32 Index = AvailableStates.IndexOfByKey(CurrentState);
	if (Index != INDEX_NONE && Index - 1 >= 0)
	{
		return AvailableStates[Index - 1];
	}
	return AvailableStates[AvailableStates.Num()-1];
}

FGameplayTag UVerseStateSubsystem::GetNextAvailableState()
{
	int32 Index = AvailableStates.IndexOfByKey(CurrentState);
	if (Index != INDEX_NONE && Index + 1 < AvailableStates.Num())
	{
		return AvailableStates[Index + 1];
	}
	return AvailableStates[0];
}

void UVerseStateSubsystem::AddAvailableState(const FGameplayTag NewState)
{
	// 이미 어레이에 존재하면 얼리 리턴
	if (AvailableStates.Contains(NewState))
	{
		UE_LOG(LogTemp,Error,TEXT("Tries To Add Already Existing VerseState!!"));
		return;
	}

	const FGameplayTag VerseState = FGameplayTag::RequestGameplayTag("VerseState");
	if (NewState.MatchesTag(VerseState))
	{
		AvailableStates.Add(NewState);
	}
	else
	{
		UE_LOG(LogTemp,Error,TEXT("Tries To Add Wrong VerseState!!"));
	}
	
}

void UVerseStateSubsystem::RemoveAvailableState(const FGameplayTag NewState)
{
	// 없는 State를 제거하려하면 얼리 리턴
	if (!AvailableStates.Contains(NewState))
	{
		UE_LOG(LogTemp,Error,TEXT("Tries To Remove Not Existing VerseState!!"));
		return;
	}
	const FGameplayTag VerseState = FGameplayTag::RequestGameplayTag("VerseState");
	if (NewState.MatchesTag(VerseState))
	{
		AvailableStates.Remove(NewState);
	}
	else
	{
		UE_LOG(LogTemp,Error,TEXT("Tries To Remove Wrong VerseState!!"));
	}
}

void UVerseStateSubsystem::MoveToNextAvailableState()
{
	SetNewState(GetNextAvailableState());
}

void UVerseStateSubsystem::MoveToPrevAvailableState()
{
	SetNewState(GetPrevAvailableState());
}
