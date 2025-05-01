// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/VJPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystem/VerseStateSubsystem.h"

void AVJPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(VJContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(Subsystem);
	Subsystem->AddMappingContext(VJContext,0);
}

void AVJPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// 프로젝트 디폴트 설정에 인핸스드인풋 컴포넌트가 설정되어 있기때문에 캐스팅 가능
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(MoveAction,ETriggerEvent::Triggered,this,&AVJPlayerController::Move);
	EnhancedInputComponent->BindAction(CameraMoveAction,ETriggerEvent::Triggered,this,&AVJPlayerController::MoveCamera);
	EnhancedInputComponent->BindAction(ChangeVerseAction,ETriggerEvent::Triggered,this,&AVJPlayerController::ChangeVerse);
	EnhancedInputComponent->BindAction(JumpAction,ETriggerEvent::Started,this,&AVJPlayerController::Jump);
	EnhancedInputComponent->BindAction(JumpAction,ETriggerEvent::Completed,this,&AVJPlayerController::StopJump);

}

void AVJPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2d InputAxisVector = InputActionValue.Get<FVector2D>();

	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);
 
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
 
	if (APawn* ControlledPawn = GetPawn())
	{
		ControlledPawn->AddMovementInput(ForwardDirection,InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection,InputAxisVector.X);
	}
	
}

void AVJPlayerController::MoveCamera(const FInputActionValue& InputActionValue)
{
	const FVector2d InputAxisVector = InputActionValue.Get<FVector2D>();

	AddYawInput(InputAxisVector.X);
	AddPitchInput(InputAxisVector.Y);
}


void AVJPlayerController::ChangeVerse(const FInputActionValue& InputActionValue)
{
	const float InputFloat = InputActionValue.Get<float>();

	UVerseStateSubsystem* VerseStateSubsystem = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UVerseStateSubsystem>();
	checkf(VerseStateSubsystem,TEXT("VerseStateSubsystem was NULL when tries to ChangeVerse"));
	// Next
	if (InputFloat > 0)
	{
		VerseStateSubsystem->MoveToNextAvailableState();
	}
	// Prev
	else if (InputFloat < 0)
	{
		VerseStateSubsystem->MoveToPrevAvailableState();
	}
}

void AVJPlayerController::Jump() 
{
	Cast<ACharacter>(GetPawn())->Jump();	
}

void AVJPlayerController::StopJump()
{
	Cast<ACharacter>(GetPawn())->StopJumping();	
}
