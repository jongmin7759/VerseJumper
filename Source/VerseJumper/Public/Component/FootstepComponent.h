// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FootstepComponent.generated.h"

class USurfaceSoundMap;
class AVJCharacterBase;

// 블루프린트에서 파생해서 쓰도록
UCLASS()
class VERSEJUMPER_API UFootstepComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFootstepComponent();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// 발소리 재생 간격 (초)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Footstep")
	float StepInterval = 0.5f;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USurfaceSoundMap> SurfaceSoundMap;
	
	TObjectPtr<AVJCharacterBase> OwnerVJCharacter;
	float TimeSinceLastStep = 0.f;

	void PlayFootstepSound(const bool bIsOnLadder);
	USoundBase* GetFootstepSound(const bool bIsOnLadder) const;
};


