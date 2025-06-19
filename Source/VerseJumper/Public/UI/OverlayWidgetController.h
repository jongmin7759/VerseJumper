// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/VJWidgetController.h"
#include "OverlayWidgetController.generated.h"

struct FDialogueRow;
struct FICMetaData;
class UInteractionComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStatusChangedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnVerseStateChangedSignature,const FGameplayTag&,NewState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnICUpdateSignature,const FICMetaData&,NewICMetaData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialogueWidgetUpdatedSignature,const FDialogueRow&,NewDialogue);


UCLASS(BlueprintType, Blueprintable)
class VERSEJUMPER_API UOverlayWidgetController : public UVJWidgetController
{
	GENERATED_BODY()
public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable)
	FOnStatusChangedSignature OnJumpBegin;
	UPROPERTY(BlueprintAssignable)
	FOnStatusChangedSignature OnJumpEnd;
	UPROPERTY(BlueprintAssignable)
	FOnStatusChangedSignature OnModifierPressed;
	UPROPERTY(BlueprintAssignable)
	FOnStatusChangedSignature OnModifierReleased;
	UPROPERTY(BlueprintAssignable)
	FOnVerseStateChangedSignature OnVerseStateSet;
	UPROPERTY(BlueprintAssignable)
	FOnStatusChangedSignature OnTargetStateSet;
	UPROPERTY(BlueprintAssignable)
	FOnICUpdateSignature OnInteractableActorDetected;
	UPROPERTY(BlueprintAssignable)
	FOnStatusChangedSignature OnInteractableActorLost;
	UPROPERTY(BlueprintAssignable)
	FOnDialogueWidgetUpdatedSignature OnDialogueUpdated;
	UPROPERTY(BlueprintAssignable)
	FOnStatusChangedSignature OnDialogueStart;
	UPROPERTY(BlueprintAssignable)
	FOnStatusChangedSignature OnDialogueEnd;

	protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Interaction")
	TObjectPtr<UInteractionComponent> CurrentInteractionComponent;
};
