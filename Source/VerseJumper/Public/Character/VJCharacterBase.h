// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Component/LadderComponent.h"
#include "GameFramework/Character.h"
#include "VJCharacterBase.generated.h"

class ULadderComponent;

UCLASS()
class VERSEJUMPER_API AVJCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	AVJCharacterBase();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void EnterLadder(ULadderComponent* NewLadder);
	UFUNCTION()
	void ExitLadder();
	
	UFUNCTION()
	bool IsNearGround() const;
	UFUNCTION()
	void SetIsOnLadder(const bool Value) {bIsOnLadder = Value;}
	UFUNCTION()
	bool GetIsOnLadder() const {return bIsOnLadder;}

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Character|Movement")
	bool bIsOnLadder = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Character|Movement")
	float GroundDetectionDistance = 50.f;

	UPROPERTY()
	TObjectPtr<ULadderComponent> CurrentLadder = nullptr;

};
