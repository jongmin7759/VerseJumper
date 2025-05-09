// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/VJWidgetController.h"
#include "OverlayWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMovementChangedSignature);

UCLASS(BlueprintType, Blueprintable)
class VERSEJUMPER_API UOverlayWidgetController : public UVJWidgetController
{
	GENERATED_BODY()
public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable)
	FOnMovementChangedSignature OnJumpBegin;
	UPROPERTY(BlueprintAssignable)
	FOnMovementChangedSignature OnLanded;

private:
	UFUNCTION()
	void HandleLanding(const FHitResult& Hit);
};
