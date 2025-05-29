// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameplayTagContainer.h"

#include "VerseStateSubsystem.generated.h"

// VerseState를 관리하기 위한 서브시스템으로
// 현재 상태 확인과 새로운 상태 설정을 할 수 있도록 만듦. 
// 새로운 상태로 변경되면 브로드캐스팅을 통해 특정 인터페이스를 사용하는 액터들이 확인 가능하도록함.
DECLARE_MULTICAST_DELEGATE_OneParam(FVerseStateChangedSignature, const FGameplayTag&);
// 타겟 상태 변경이 될 때 브로드캐스팅하기 (위젯 컨트롤러에서 바인딩하려고)
DECLARE_MULTICAST_DELEGATE(FOnTargetStateChangedDelegate);

UCLASS()
class VERSEJUMPER_API UVerseStateSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure)
	FGameplayTag GetCurrentState() const;
	UFUNCTION(BlueprintPure)
	FGameplayTag GetTargetState() const;

	FVerseStateChangedSignature VerseStateChanged;
	FOnTargetStateChangedDelegate OnTargetStateChanged;

	// Initial State 설정
	void InitializeVerseState(const FGameplayTag& InitialState);
	
	// 새로운 스테이트 설정 & 브로드캐스팅
	UFUNCTION(BlueprintCallable)
	void SetNewState(const FGameplayTag& NewState);


	// AvailableState 순회
	UFUNCTION(BlueprintPure)
	FGameplayTag GetPrevAvailableState(const FGameplayTag& Tag = FGameplayTag()) const;
	UFUNCTION(BlueprintPure)
	FGameplayTag GetNextAvailableState(const FGameplayTag& Tag = FGameplayTag()) const;
	
	// AvailableState 추가
	UFUNCTION(BlueprintCallable)
	void AddAvailableState(const FGameplayTag NewState);
	// AvailableState 제거
	UFUNCTION(BlueprintCallable)
	void RemoveAvailableState(const FGameplayTag NewState);
	// CurrentState를 TargetState로 변경
	UFUNCTION(BlueprintCallable)
	void MoveToTargetState();
	// TargetState 다음 이동 (인덱스++)
	UFUNCTION(BlueprintCallable)
	void SetTargetStateToNext();
	// TargetState 이전 이동 (인덱스--)
	UFUNCTION(BlueprintCallable)
	void SetTargetStateToPrev();
	

private:
	// 현재 상태 저장하기 (초기화할 때 0이나 세이브 데이터 기준으로 세팅)
	int32 CurrentState = -1;
	int32 TargetState = -1;

	
	// 현 상태에서 접근 가능한 상태를 관리
	// TODO : VerseState DataTable과 연동해서 초기화하기
	UPROPERTY(EditDefaultsOnly)
	TArray<FGameplayTag> AvailableStates = {FGameplayTag::RequestGameplayTag("VerseState.AlphaVerse")};
};



