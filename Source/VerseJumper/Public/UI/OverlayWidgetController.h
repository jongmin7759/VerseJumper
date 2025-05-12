// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/VJWidgetController.h"
#include "OverlayWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStatusChangedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnVerseStateChangedSignature,const FGameplayTag&,NewState);

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
	FOnStatusChangedSignature OnLanded;
	UPROPERTY(BlueprintAssignable)
	FOnStatusChangedSignature OnModifierPressed;
	UPROPERTY(BlueprintAssignable)
	FOnStatusChangedSignature OnModifierReleased;
	UPROPERTY(BlueprintAssignable)
	FOnVerseStateChangedSignature OnVerseStateSet;
	UPROPERTY(BlueprintAssignable)
	FOnStatusChangedSignature OnTargetStateSet;

private:
	UFUNCTION()
	void HandleLanding(const FHitResult& Hit);
};
