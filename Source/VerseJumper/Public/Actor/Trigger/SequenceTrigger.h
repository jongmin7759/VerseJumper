// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/TriggerBox.h"
#include "SequenceTrigger.generated.h"

class AVJPlayerController;
class ULevelSequencePlayer;
class ULevelSequence;
/**
 *  시퀀스 플레이어 재생용 트리거 박스
 *  고유 시퀀스 태그를(게임플레이 태그) 가지고 있으며
 *  재생하면 플레이어의 태그 컨테이너에 저장해서 중복 재생되지 않도록 함
 */
UCLASS()
class VERSEJUMPER_API ASequenceTrigger : public ATriggerBox
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	FGameplayTag SequenceTag;
	UPROPERTY(EditAnywhere)
	TObjectPtr<ULevelSequence> Sequence;
	// 시퀀스 끝나면 자동 종료 -> SequenceStopped 호출하도록
	UPROPERTY(EditAnywhere)
	bool bAutoFinish = true;
	UFUNCTION()
	void PlaySequence(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);


protected:
	virtual void BeginPlay() override;
private:
	TObjectPtr<ULevelSequencePlayer> LevelSequencePlayer;
	TWeakObjectPtr<AVJPlayerController> VJPlayerController;
	UFUNCTION()
	void FinishSequence();
};
