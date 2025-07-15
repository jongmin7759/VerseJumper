// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/TriggerBox.h"
#include "TutorialTrigger.generated.h"



/**
 *  Tutorial 위젯을 표시할 트리거
 *  고유 태그를(게임플레이 태그) 가지고 있으며
 *  이미 봤으면 스킵하도록 체크
 */
UCLASS()
class VERSEJUMPER_API ATutorialTrigger : public ATriggerBox
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	bool bActivateOnBegin = true;
	UPROPERTY(EditAnywhere)
	FGameplayTag TutorialTag;
	UFUNCTION()
	void ShowTutorial(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	void RemoveTutorial(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
protected:
	virtual void BeginPlay() override;
};
