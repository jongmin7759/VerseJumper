// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/VJPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Character/VJPlayerCharacter.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystem/VerseStateSubsystem.h"
#include "UI/VJHUD.h"

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
	EnhancedInputComponent->BindAction(VerseJumpAction,ETriggerEvent::Started,this,&AVJPlayerController::VerseJump);
	EnhancedInputComponent->BindAction(ModifierAction,ETriggerEvent::Started,this,&AVJPlayerController::ModifierPressed);
	EnhancedInputComponent->BindAction(ModifierAction,ETriggerEvent::Completed,this,&AVJPlayerController::ModifierReleased);

}

void AVJPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	PlayerCharacter = Cast<AVJPlayerCharacter>(InPawn);
	checkf(PlayerCharacter,TEXT("Player character is null"));
	if (USphereComponent* JumpBlocker = PlayerCharacter->GetJumpBlocker())
	{
		JumpBlocker->OnComponentBeginOverlap.AddDynamic(this,&AVJPlayerController::BlockJump);
	}

	if (AVJHUD* HUD = Cast<AVJHUD>(GetHUD()))
	{
		HUD->InitOverlay(this);
	}
		
}

void AVJPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2d InputAxisVector = InputActionValue.Get<FVector2D>();
	if (PlayerCharacter)
	{
		PlayerCharacter->HandleMovementInput(InputAxisVector);
	}
}

void AVJPlayerController::MoveCamera(const FInputActionValue& InputActionValue)
{
	const FVector2d InputAxisVector = InputActionValue.Get<FVector2D>();

	if (PlayerCharacter)
	{
		PlayerCharacter->HandleLookInput(InputAxisVector);
	}
}


void AVJPlayerController::ChangeVerse(const FInputActionValue& InputActionValue)
{
	// Modifier가 먼저 눌러져있는 상태에서만 동작하도록
	if (PlayerCharacter == nullptr) return;
	if (PlayerCharacter->IsModifierPressed() == false) return;
	
	const float InputFloat = InputActionValue.Get<float>();

	UVerseStateSubsystem* VerseStateSubsystem = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UVerseStateSubsystem>();
	checkf(VerseStateSubsystem,TEXT("VerseStateSubsystem was NULL when tries to ChangeVerse"));
	// Next
	if (InputFloat > 0)
	{
		VerseStateSubsystem->SetTargetStateToNext();
	}
	// Prev
	else if (InputFloat < 0)
	{
		VerseStateSubsystem->SetTargetStateToPrev();
	}
}

// bool AVJPlayerController::CanJump() const
// {
// 	if (PlayerCharacter)
// 	{
// 		// 캐릭터가 점프할 수 있는 상황 &&  Modifier가 눌려있지 않은 상황
// 		return PlayerCharacter->CanJump();
// 	}
// 	return false;
// }
void AVJPlayerController::Jump() 
{
	// Jump 시작할 때 JumpBlocker의 콜리전을 켜두고 Jump
	if (PlayerCharacter)
	{
		// 점프할 수 없는 상황이면 얼리 리턴
		if (PlayerCharacter->CanJump() == false) return;
		
		if (USphereComponent* JumpBlocker = PlayerCharacter->GetJumpBlocker())
		{
			JumpBlocker->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		}
		PlayerCharacter->Jump();
	}
}

void AVJPlayerController::StopJump()
{
	// Jump가 끝날 때는 콜리전 다시 꺼두기
	if (PlayerCharacter)
	{
		if (USphereComponent* JumpBlocker = PlayerCharacter->GetJumpBlocker())
		{
			JumpBlocker->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
		PlayerCharacter->StopJumping();	
	}
}

void AVJPlayerController::VerseJump()
{
	if (PlayerCharacter == nullptr) return;
	// Modifier가 먼저 눌러져있는 상태에서만 동작하도록
	if (PlayerCharacter->IsModifierPressed() == false) return;
	
	UVerseStateSubsystem* VerseStateSubsystem = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UVerseStateSubsystem>();
	checkf(VerseStateSubsystem,TEXT("VerseStateSubsystem was NULL when tries to ChangeVerse"));
	VerseStateSubsystem->MoveToTargetState();
}

void AVJPlayerController::ModifierPressed()
{
	if (PlayerCharacter)
	{
		PlayerCharacter->PressModifier();	
	}
}

void AVJPlayerController::ModifierReleased()
{
	if (PlayerCharacter)
	{
		PlayerCharacter->ReleaseModifier();	
	}
}

void AVJPlayerController::BlockJump(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor != GetPawn())
	{
		StopJump();
	}
}
