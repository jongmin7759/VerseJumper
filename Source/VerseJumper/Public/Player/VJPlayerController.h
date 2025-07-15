// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "VJPlayerController.generated.h"

class UPlayerVerseStateComponent;
class UDialogueManager;
class UInteractionComponent;
class AVJPlayerCharacter;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnInteractableDetectedSignature, UInteractionComponent*);
DECLARE_MULTICAST_DELEGATE(FOnActionSignature);


UCLASS()
class VERSEJUMPER_API AVJPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AVJPlayerController();
	
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> VJContext;
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> DialContext;
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
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> InteractAction;
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> AdvanceDialAction;
	
	// Interaction
	FOnInteractableDetectedSignature OnInteractableActorDetected;
	FOnActionSignature OnInteractableActorLost;

	// Dialogue
	UDialogueManager* GetDialogueManager() const {return DialogueManager;}

	// VerseState
	UPlayerVerseStateComponent* GetPlayerVerseStateComponent() const {return PlayerVerseStateComponent;}

	// Sequence
	FOnActionSignature OnSequncePlaying;
	FOnActionSignature OnSequnceStopped;

	// CutScene 처리
	UFUNCTION()
	void HandleSequnecePlaying() const;
	UFUNCTION()
	void HandleSequneceStopped() const;

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
	void Interact();
	void AdvanceDial();
	UFUNCTION()
	void BlockJump(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	void SwapIMC(UInputMappingContext* NewIMC) const;
	// 입력 전부 막기
	UFUNCTION()
	void BlockAllInput() const;
	// 입력 복원 (Default IMC로)
	UFUNCTION()
	void RestoreDefaultInput() const;
	
	UPROPERTY()
	TWeakObjectPtr<AVJPlayerCharacter> PlayerCharacter;

	// Highlight Interface 처리
	void UpdateHighlightStates();
	TSet<TWeakObjectPtr<AActor>> LastHighlightCandidates;

	// Interaction 처리
	void OnActorDetected(AActor* NewActor);
	TWeakObjectPtr<UInteractionComponent> CurrentInteractionComponent;
	void ClearInteraction();

	// Dialogue 처리
	void InitDialogueManager();
	UPROPERTY(EditAnywhere)
	TSubclassOf<UDialogueManager> DialogueManagerClass;
	UPROPERTY()
	TObjectPtr<UDialogueManager> DialogueManager;
	void HandleDialogueStart();
	void HandleDialogueEnd();


	// VerseStateComponent
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,meta=(AllowPrivateAccess=true),Category="VerseState")
	TObjectPtr<UPlayerVerseStateComponent> PlayerVerseStateComponent;
	
};

