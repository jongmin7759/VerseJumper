#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "PlayerVerseStateComponent.generated.h"

// 타겟 상태 변경이 될 때 브로드캐스팅하기 (위젯 컨트롤러에서 바인딩하려고)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnTargetStateChangedSignature, const FGameplayTag&);


///
/// 플레이어가 직접 이동 가능한 VerseState를 따로 관리하기 위해서 만든 컴포넌트로
/// 외부 상태 전환 액터는 Available State와 무관하게 이동하고 (Subsystem 직접 호출)
/// 플레이어는 AvailableState를 고려해서 이동하도록
/// 기존 VerseStateSubsystem에 있던 Available State, Target State 부분을 분리시킨 것
///
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VERSEJUMPER_API UPlayerVerseStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPlayerVerseStateComponent();

	UFUNCTION(BlueprintPure)
	FGameplayTag GetTargetState() const;

	FOnTargetStateChangedSignature OnTargetStateChanged;

	// Initial State 설정
	void InitializeVerseState(const FGameplayTag& InitialState);

	// AvailableState 순회
	UFUNCTION(BlueprintPure)
	FGameplayTag GetPrevAvailableState() const;
	UFUNCTION(BlueprintPure)
	FGameplayTag GetNextAvailableState() const;

	// AvailableState 추가
	UFUNCTION(BlueprintCallable)
	void AddAvailableState(const FGameplayTag NewState);
	
	// CurrentState를 TargetState로 변경
	UFUNCTION(BlueprintCallable)
	void MoveToTargetState();
	// TargetState 다음 이동 (인덱스++)
	UFUNCTION(BlueprintCallable)
	void SetTargetStateToNext();
	// TargetState 이전 이동 (인덱스--)
	UFUNCTION(BlueprintCallable)
	void SetTargetStateToPrev();

	int32 GetCurrentStateIndex() const;

	TArray<FGameplayTag> GetAvailableStates() const {return AvailableStates;}
	void SetAvailableStates(const TArray<FGameplayTag>& NewStates) {AvailableStates = NewStates;}

protected:
	virtual void BeginPlay() override;

private:
	int32 TargetStateIndex = -1;

	// 현 상태에서 접근 가능한 상태를 관리
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(AllowPrivateAccess=true), Category="Player|VerseState")
	TArray<FGameplayTag> AvailableStates = {FGameplayTag::RequestGameplayTag("VerseState.AlphaVerse")};

};
