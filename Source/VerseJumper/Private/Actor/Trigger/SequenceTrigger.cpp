// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Trigger/SequenceTrigger.h"

#include "LevelSequencePlayer.h"
#include "Character/VJPlayerCharacter.h"
#include "Components/ShapeComponent.h"
#include "Player/VJPlayerController.h"

void ASequenceTrigger::BeginPlay()
{
	Super::BeginPlay();
	if (!GetCollisionComponent()->OnComponentBeginOverlap.IsBound())
	{
		GetCollisionComponent()->OnComponentBeginOverlap.AddDynamic(this,&ASequenceTrigger::PlaySequence);
	}
}

void ASequenceTrigger::PlaySequence(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AVJPlayerCharacter* PlayerCharacter = Cast<AVJPlayerCharacter>(OtherActor);
	if (PlayerCharacter == nullptr) return;

	// 이미 재생된 기록이 있다면 재생 X
	if (PlayerCharacter->SavedTags.HasTag(SequenceTag))
	{
		return;
	}

	// 아니라면 시퀀스 재생, 태그 추가
	ALevelSequenceActor* LevelSequenceActor;
	LevelSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), Sequence, FMovieSceneSequencePlaybackSettings(),LevelSequenceActor);

	if (LevelSequencePlayer->IsValid())
	{
		LevelSequencePlayer->Play();
		PlayerCharacter->SavedTags.AddTag(SequenceTag);
		if (AVJPlayerController* PlayerController = Cast<AVJPlayerController>(PlayerCharacter->GetController()))
		{
			// 컨트롤러에게 전달
			VJPlayerController = PlayerController;
			VJPlayerController->HandleSequnecePlaying();
			if (LevelSequencePlayer->OnFinished.IsBound())
			{
				LevelSequencePlayer->OnFinished.RemoveAll(this);
			}
			LevelSequencePlayer->OnFinished.AddDynamic(this,&ASequenceTrigger::FinishSequence);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to create level sequence player"));
	}
	
}

void ASequenceTrigger::FinishSequence() 
{
	if (VJPlayerController.IsValid())
	{
		VJPlayerController->HandleSequneceStopped();
	}
}
