#include "Component/PlayerVerseStateComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Subsystem/VerseStateSubsystem.h"

UPlayerVerseStateComponent::UPlayerVerseStateComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


void UPlayerVerseStateComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

int32 UPlayerVerseStateComponent::GetCurrentStateIndex() const
{
	if (UVerseStateSubsystem* VerseStateSubsystem = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UVerseStateSubsystem>())
	{
		FGameplayTag CurrentState = VerseStateSubsystem->GetCurrentState();
		return AvailableStates.IndexOfByKey(CurrentState);
	}

	return INDEX_NONE;
}

void UPlayerVerseStateComponent::InitializeVerseState(const FGameplayTag& InitialState)
{
	if (UVerseStateSubsystem* VerseStateSubsystem = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UVerseStateSubsystem>())
	{
		VerseStateSubsystem->SetNewState(InitialState);
	}
	
	int32 CurrentStateIndex = GetCurrentStateIndex();
	if (CurrentStateIndex != INDEX_NONE)
	{
		TargetStateIndex = CurrentStateIndex;
	}
	else
	{
		TargetStateIndex = 0;
	}

	OnTargetStateChanged.Broadcast(AvailableStates[TargetStateIndex]);

}


FGameplayTag UPlayerVerseStateComponent::GetTargetState() const
{
	if (TargetStateIndex < 0 || TargetStateIndex >= AvailableStates.Num())
	{
		UE_LOG(LogTemp,Error,TEXT("Invalid Target State Index"));
		return FGameplayTag();
	}
	return AvailableStates[TargetStateIndex];
}


FGameplayTag UPlayerVerseStateComponent::GetPrevAvailableState() const
{
	if (AvailableStates.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("AvailableStates is empty!"));
		return FGameplayTag::EmptyTag;
	}
	
	int32 PrevIndex = TargetStateIndex - 1;
	if (AvailableStates.IsValidIndex(PrevIndex))
	{
		return AvailableStates[PrevIndex];
	}
	return AvailableStates.Last();
}

FGameplayTag UPlayerVerseStateComponent::GetNextAvailableState() const
{
	if (AvailableStates.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("AvailableStates is empty!"));
		return FGameplayTag::EmptyTag;
	}
	
	int32 NextIndex = TargetStateIndex + 1;
	if (AvailableStates.IsValidIndex(NextIndex))
	{
		return AvailableStates[NextIndex];
	}
	return AvailableStates[0];
}

void UPlayerVerseStateComponent::AddAvailableState(const FGameplayTag NewState)
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

void UPlayerVerseStateComponent::MoveToTargetState()
{
	if (UVerseStateSubsystem* VerseStateSubsystem = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UVerseStateSubsystem>())
	{
		VerseStateSubsystem->SetNewState(GetTargetState());
	}
}


void UPlayerVerseStateComponent::SetTargetStateToNext()
{
	// 변경할 상태가 없으면 얼리 리턴
	if (AvailableStates.Num() < 2) return;
	
	if (TargetStateIndex != INDEX_NONE && TargetStateIndex + 1 < AvailableStates.Num())
	{
		TargetStateIndex = TargetStateIndex + 1;
	}
	else
	{
		TargetStateIndex = 0;
	}
	OnTargetStateChanged.Broadcast(AvailableStates[TargetStateIndex]);
}

void UPlayerVerseStateComponent::SetTargetStateToPrev()
{
	// 변경할 상태가 없으면 얼리 리턴
	if (AvailableStates.Num() < 2) return;
	
	if (TargetStateIndex != INDEX_NONE && TargetStateIndex - 1 >= 0)
	{
		TargetStateIndex = TargetStateIndex - 1;
	}
	else
	{
		TargetStateIndex = AvailableStates.Num()-1;
	}
	OnTargetStateChanged.Broadcast(AvailableStates[TargetStateIndex]);
}
