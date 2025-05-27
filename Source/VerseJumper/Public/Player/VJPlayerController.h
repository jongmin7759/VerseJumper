// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "VJPlayerController.generated.h"

class AVJPlayerCharacter;
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
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> JumpAction;
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> VerseJumpAction;
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> ModifierAction;

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void Tick(float DeltaTime) override;

private:
	void Move(const FInputActionValue& InputActionValue);
	void MoveCamera(const FInputActionValue& InputActionValue);
	void ChangeVerse(const FInputActionValue& InputActionValue);
	//bool CanJump() const;
	void Jump();
	void StopJump();
	void VerseJump();
	void ModifierPressed();
	void ModifierReleased();
	UFUNCTION()
	void BlockJump(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UPROPERTY()
	TWeakObjectPtr<AVJPlayerCharacter> PlayerCharacter;

	// Highlight Interface 구현을 위한 부분
	void UpdateHighlightStates();
	TSet<TWeakObjectPtr<AActor>> LastHighlightCandidates;
	
};

