// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Component/LadderComponent.h"
#include "GameFramework/Character.h"
#include "Interface/VerseStateInterface.h"
#include "VJCharacterBase.generated.h"

class AVJLadderActor;
class UFootstepComponent;

UCLASS()
class VERSEJUMPER_API AVJCharacterBase : public ACharacter, public IVerseStateInterface
{
	GENERATED_BODY()

public:
	AVJCharacterBase();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// PlayerCharacter에서 오버라이드하기 위해 virtual
	UFUNCTION()
	virtual void EnterLadder(AVJLadderActor* NewLadder);
	UFUNCTION()
	void ExitLadder();
	AVJLadderActor* GetCurrentLadder() const {return CurrentLadder;}
	
	UFUNCTION(BlueprintPure)
	bool IsNearGround() const;

	UFUNCTION()
	void SetIsOnLadder(const bool Value) {bIsOnLadder = Value;}
	UFUNCTION(BlueprintPure)
	bool GetIsOnLadder() const {return bIsOnLadder;}

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Character|Movement")
	bool bIsOnLadder = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character|Movement")
	float GroundDetectionDistance = 50.f;

	UPROPERTY()
	TObjectPtr<AVJLadderActor> CurrentLadder = nullptr;

	// IVerseStateInterface
	virtual void Internal_OnVerseStateChanged(const FGameplayTag& NewState) override;

private:
	UPROPERTY(VisibleAnywhere, Category="Character|Footstep")
	TObjectPtr<UFootstepComponent> FootstepComponent = nullptr;
};
