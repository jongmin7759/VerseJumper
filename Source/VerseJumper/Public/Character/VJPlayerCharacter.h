// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/VJCharacterBase.h"
#include "VJPlayerCharacter.generated.h"

class USphereComponent;

DECLARE_MULTICAST_DELEGATE(FOnActionDelegate);

UCLASS()
class VERSEJUMPER_API AVJPlayerCharacter : public AVJCharacterBase
{
	GENERATED_BODY()
public:
	AVJPlayerCharacter();

	USphereComponent* GetJumpBlocker() const {return JumpBlocker;}
	bool IsModifierPressed() const {return bIsModifierPressed;}

	void HandleMovementInput(const FVector2D& Input);
	void HandleLookInput(const FVector2D& Input);

	FOnActionDelegate OnJumpBegin;
	// 점프가 실행됐을 때 (위젯 컨트롤러에 브로드캐스팅하기위해 오버라이드)
	virtual void OnJumped_Implementation() override;
	FOnActionDelegate OnJumpEnd;
	virtual void EnterLadder(AVJLadderActor* NewLadder) override;

	virtual void Landed(const FHitResult& Hit) override;
	
	FOnActionDelegate OnModifierPressed;
	UFUNCTION()
	void PressModifier();
	FOnActionDelegate OnModifierReleased;
	UFUNCTION()
	void  ReleaseModifier();

protected:
	// 점프 가능한 상황인지 판단 , ACharacter의 함수 오버라이드
	virtual bool CanJumpInternal_Implementation() const override; 
	
	// Jump할 때 머리가 닿으면 멈추도록하기 위해 머리에 붙일 콜리전 추가
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly , Category = "Player|Collision")
	TObjectPtr<USphereComponent> JumpBlocker;

	// SFX
	UPROPERTY(EditAnywhere, Category="Player|SFX")
	TObjectPtr<USoundBase> JumpSound;
	UPROPERTY(EditAnywhere, Category="Player|SFX")
	TObjectPtr<USoundBase> LandingSound;
	UPROPERTY(EditAnywhere, Category="Player|SFX")
	TObjectPtr<USoundBase> VerseJumpSound;

	// IVerseStateInterface
	virtual void Internal_OnVerseStateChanged(const FGameplayTag& NewState) override;

private:
	bool bIsModifierPressed = false;

	void PlaySFX(USoundBase* SoundBase) const;
};
