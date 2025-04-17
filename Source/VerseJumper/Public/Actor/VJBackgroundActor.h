// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/VerseStateResponsiveActor.h"
#include "VJBackgroundActor.generated.h"

/**
 * 
 */
UCLASS()
class VERSEJUMPER_API AVJBackgroundActor : public AVerseStateResponsiveActor
{
	GENERATED_BODY()

public:
	AVJBackgroundActor();

protected:
	virtual void Internal_OnVerseStateChanged(const FGameplayTag& NewState) override;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UInstancedStaticMeshComponent> InstancedStaticMeshComponent = nullptr;
	
};
