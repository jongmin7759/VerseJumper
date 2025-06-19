// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/DialogueManager.h"

#include "Component/DialogueComponent.h"

void UDialogueManager::StartDialogue(UDialogueComponent* DialogueComponent)
{
	if (!DialogueComponent) return;
	
	CurrentDialogueComponent = DialogueComponent;
	bIsActive = true;
	CurrentLineID = DialogueComponent->GetStartingLineID();
	OnDialogueStart.Broadcast();
	UpdateDialogue(GetDialogueRowByCurrentLine());
}

void UDialogueManager::AdvanceDialogue()
{
	if (HasNextLine())
	{
		if (const FDialogueRow* RawPtr = GetDialogueRowByCurrentLine())
		{
			CurrentLineID = RawPtr->NextLine;
		}
		
		UpdateDialogue(GetDialogueRowByCurrentLine());
	}
	else
	{
		EndDialogue();
	}
}

void UDialogueManager::EndDialogue()
{
	CurrentDialogueComponent = nullptr;
	bIsActive = false;

	if (OnDialogueEnd.IsBound())
	{
		OnDialogueEnd.Broadcast();
	}
}

bool UDialogueManager::HasNextLine() const
{
	if (const FDialogueRow* RawPtr = GetDialogueRowByCurrentLine())
	{
		return !RawPtr->NextLine.IsNone();
	}
	
	return false;
}

const FDialogueRow* UDialogueManager::GetDialogueRowByCurrentLine() const
{
	return DialogueTable->FindRow<FDialogueRow>(CurrentLineID,TEXT(""));
}

void UDialogueManager::UpdateDialogue(const FDialogueRow* DialogueRow) const
{
	if (!OnDialogueUpdated.IsBound()) return;
	
	if (DialogueRow)
	{
		OnDialogueUpdated.Execute(*DialogueRow);
	}
	else
	{
		// 임시 객체 전달
		OnDialogueUpdated.Execute(FDialogueRow());
	}
}
