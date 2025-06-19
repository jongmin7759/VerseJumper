// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "VerseStateInterface.generated.h"

struct FGameplayTag;

UINTERFACE(MinimalAPI)
class UVerseStateInterface : public UInterface
{
	GENERATED_BODY()
};

// TODO: 초기 의도와 다르게 인터페이스를 사용할 필요성이 희미해져서 굳이 인터페이스화하지 않아도 될 것 같음
// 기존 사용중인 이 인터페이스 구현 클래스들을 정리할 필요가 있음
class VERSEJUMPER_API IVerseStateInterface
{
	GENERATED_BODY()

public:
	virtual void Internal_OnVerseStateChanged(const FGameplayTag& NewState) = 0;
};
