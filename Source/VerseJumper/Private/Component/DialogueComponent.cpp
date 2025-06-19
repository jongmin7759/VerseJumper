// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/DialogueComponent.h"

#include "Component/DialogueManager.h"
#include "Player/VJPlayerController.h"

UDialogueComponent::UDialogueComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UDialogueComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UDialogueComponent::SetStartingLineID(FName NewStartingLineID)
{
	StartingLineID = NewStartingLineID;
}

void UDialogueComponent::BeginDialogue(APlayerController* InstigatorController , FName OverridingStartingLineID)
{
	if (!OverridingStartingLineID.IsEqual(FName("NotOverride")))
	{
		SetStartingLineID(OverridingStartingLineID);
	}

	// DialogueManager에게 시작 라인 전달하고 Manager가 대화 흐름 처리함
	if (AVJPlayerController* VJPC =  Cast<AVJPlayerController>(InstigatorController))
	{
		VJPC->GetDialogueManager()->StartDialogue(this);
	}
	
}


