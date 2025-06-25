// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameplayTagContainer.h"

#include "VerseStateSubsystem.generated.h"

// VerseState를 관리하기 위한 서브시스템으로
// 현재 상태 확인과 새로운 상태 설정을 할 수 있도록 만듦. 
// 새로운 상태로 변경되면 브로드캐스팅을 통해 액터들이 확인 가능하도록함.
DECLARE_MULTICAST_DELEGATE_OneParam(FVerseStateChangedSignature, const FGameplayTag&);


UCLASS()
class VERSEJUMPER_API UVerseStateSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure)
	FGameplayTag GetCurrentState() const {return CurrentState;}
	
	FVerseStateChangedSignature VerseStateChanged;

	// 새로운 스테이트 설정 & 브로드캐스팅
	UFUNCTION(BlueprintCallable)
	void SetNewState(const FGameplayTag& NewState);

private:
	// 현재 상태 저장하기 (초기화할 때 0이나 세이브 데이터 기준으로 세팅)
	FGameplayTag CurrentState;
};



