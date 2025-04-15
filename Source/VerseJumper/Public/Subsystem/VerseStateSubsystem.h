// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameplayTagContainer.h"

#include "VerseStateSubsystem.generated.h"

// VerseState를 관리하기 위한 서브시스템으로
// 현재 상태 확인과 새로운 상태 설정을 할 수 있도록 만듦. 
// 새로운 상태로 변경되면 브로드캐스팅을 통해 특정 인터페이스를 사용하는 액터들이 확인 가능하도록함.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FVerseStateChangedSignature, const FGameplayTag&, NewState);

UCLASS()
class VERSEJUMPER_API UVerseStateSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	FGameplayTag GetCurrentState() const { return CurrentState; }

	// 새로운 스테이트 설정 & 브로드캐스팅
	UFUNCTION(BlueprintCallable)
	void SetNewState(FGameplayTag NewState);

	UPROPERTY(BlueprintAssignable)
	FVerseStateChangedSignature VerseStateChanged;

private:
	// 현재 상태 저장하기
	FGameplayTag CurrentState;
};
