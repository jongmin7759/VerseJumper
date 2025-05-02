// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/VJCharacterBase.h"
#include "VJPlayerCharacter.generated.h"

class USphereComponent;
/**
 * 
 */
UCLASS()
class VERSEJUMPER_API AVJPlayerCharacter : public AVJCharacterBase
{
	GENERATED_BODY()
public:
	AVJPlayerCharacter();

	USphereComponent* GetJumpBlocker() const {return JumpBlocker;}

	void HandleMovementInput(const FVector2D& Input);
	void HandleLookInput(const FVector2D& Input);
protected:
	// 점프 가능한 상황인지 판단 , ACharacter의 함수 오버라이드
	virtual bool CanJumpInternal_Implementation() const override; 
	
	// Jump할 때 머리가 닿으면 멈추도록하기 위해 머리에 붙일 콜리전 추가
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly , Category = "Player|Collision")
	TObjectPtr<USphereComponent> JumpBlocker;
};
