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

/**
 * 
 */
class VERSEJUMPER_API IVerseStateInterface
{
	GENERATED_BODY()

public:
	virtual void Internal_OnVerseStateChanged(const FGameplayTag& NewState) = 0;
};
