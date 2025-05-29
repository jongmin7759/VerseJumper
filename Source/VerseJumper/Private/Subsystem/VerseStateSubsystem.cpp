// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/VerseStateSubsystem.h"

FGameplayTag UVerseStateSubsystem::GetCurrentState() const
{
	if (CurrentState < 0 || CurrentState >= AvailableStates.Num())
	{
		UE_LOG(LogTemp,Error,TEXT("Invalid Current State Index"));
		return FGameplayTag();
	}
	return AvailableStates[CurrentState];
}

FGameplayTag UVerseStateSubsystem::GetTargetState() const
{
	if (TargetState < 0 || TargetState >= AvailableStates.Num())
	{
		UE_LOG(LogTemp,Error,TEXT("Invalid Target State Index"));
		return FGameplayTag();
	}
	return AvailableStates[TargetState];
}

void UVerseStateSubsystem::SetNewState(const FGameplayTag& NewState)
{
	int32 NewIndex = AvailableStates.IndexOfByKey(NewState);
	// 현재 상태와 같거나 접근 불가한 상태라면 얼리 리턴
	if (NewIndex == CurrentState || NewIndex == INDEX_NONE) return;
	
	const FGameplayTag VerseState = FGameplayTag::RequestGameplayTag("VerseState");
	if (NewState.MatchesTag(VerseState))
	{
		CurrentState = NewIndex;
		// 브로드캐스팅
		VerseStateChanged.Broadcast(NewState);
	}
	else
	{
		UE_LOG(LogTemp,Error,TEXT("Tries To Set Wrong VerseState!!"));
	}
}

void UVerseStateSubsystem::InitializeVerseState(const FGameplayTag& InitialState)
{
	SetNewState(InitialState);
	TargetState = CurrentState;
}

FGameplayTag UVerseStateSubsystem::GetPrevAvailableState(const FGameplayTag& Tag) const
{
	int32 Index = 0;
	const FGameplayTag VerseState = FGameplayTag::RequestGameplayTag("VerseState");
	//Tag를 넘기지 않았거나 VerseState가 아닌 잘못된 인자를 넘겼다면 현재 상태를 기준으로 반환
	if (Tag.MatchesTag(FGameplayTag::EmptyTag) || Tag.MatchesTag(VerseState) == false)
	{
		Index = CurrentState;		
	}
	else
	{
		Index = AvailableStates.IndexOfByKey(Tag);
	}
	 
	if (Index != INDEX_NONE && Index - 1 >= 0)
	{
		return AvailableStates[Index - 1];
	}
	return AvailableStates.Last();
}

FGameplayTag UVerseStateSubsystem::GetNextAvailableState(const FGameplayTag& Tag) const
{
	int32 Index = 0;
	const FGameplayTag VerseState = FGameplayTag::RequestGameplayTag("VerseState");
	// Tag를 넘기지 않았거나 VerseState가 아닌 잘못된 인자를 넘겼다면 현재 상태를 기준으로 반환
	if (Tag.MatchesTagExact(FGameplayTag::EmptyTag) || Tag.MatchesTag(VerseState) == false)
	{
		Index = CurrentState;		
	}
	else
	{
		Index = AvailableStates.IndexOfByKey(Tag);
	}
	
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

void UVerseStateSubsystem::MoveToTargetState()
{
	SetNewState(GetTargetState());
	CurrentState = TargetState;
}

void UVerseStateSubsystem::SetTargetStateToNext()
{
	if (TargetState != INDEX_NONE && TargetState + 1 < AvailableStates.Num())
	{
		TargetState = TargetState + 1;
	}
	else
	{
		TargetState = 0;
	}
	OnTargetStateChanged.Broadcast();
}

void UVerseStateSubsystem::SetTargetStateToPrev()
{
	if (TargetState != INDEX_NONE && TargetState - 1 >= 0)
	{
		TargetState = TargetState - 1;
	}
	else
	{
		TargetState = AvailableStates.Num()-1;
	}
	OnTargetStateChanged.Broadcast();
}
