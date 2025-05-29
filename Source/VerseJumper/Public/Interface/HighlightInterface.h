// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HighlightInterface.generated.h"

UINTERFACE(MinimalAPI)
class UHighlightInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *	액터의 외곽선 강조 인터페이스
 */
class VERSEJUMPER_API IHighlightInterface
{
	GENERATED_BODY()

public:
	virtual void HighlightActor() = 0;
	virtual void UnHighlightActor() = 0;
	virtual bool IsHighlighted() const = 0;
	virtual void SetHighlightColor(int32 Value) = 0;
};
