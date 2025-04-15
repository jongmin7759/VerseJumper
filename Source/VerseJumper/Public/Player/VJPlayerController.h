// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "VJPlayerController.generated.h"

class UInputAction;
class UInputMappingContext;
struct FInputActionValue;
/**
 * 
 */
UCLASS()
class VERSEJUMPER_API AVJPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> VJContext;
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MoveAction;
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> CameraMoveAction;
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> ChangeVerseAction;

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	void Move(const FInputActionValue& InputActionValue);
	void MoveCamera(const FInputActionValue& InputActionValue);
	void ChangeVerse(const FInputActionValue& InputActionValue);

};
