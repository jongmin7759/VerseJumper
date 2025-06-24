// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/VJPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Character/VJPlayerCharacter.h"
#include "Component/DialogueManager.h"
#include "Component/InteractionComponent.h"
#include "Components/SphereComponent.h"
#include "Interface/HighlightInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystem/VerseStateSubsystem.h"
#include "UI/VJHUD.h"
#include "VerseJumper/VerseJumper.h"

void AVJPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(VJContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(Subsystem);
	Subsystem->AddMappingContext(VJContext,0);

	// VerseState Initialize
	UVerseStateSubsystem* VerseStateSubsystem = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UVerseStateSubsystem>();
	checkf(VerseStateSubsystem,TEXT("VerseStateSubsystem was NULL on AVJPlayerController::BeginPlay"));
	VerseStateSubsystem->InitializeVerseState(FGameplayTag::RequestGameplayTag("VerseState.AlphaVerse"));
	
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
	EnhancedInputComponent->BindAction(InteractAction,ETriggerEvent::Started,this,&AVJPlayerController::Interact);
	EnhancedInputComponent->BindAction(AdvanceDialAction,ETriggerEvent::Started,this,&AVJPlayerController::AdvanceDial);

}

void AVJPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	PlayerCharacter = Cast<AVJPlayerCharacter>(InPawn);
	
	if (USphereComponent* JumpBlocker = PlayerCharacter->GetJumpBlocker())
	{
		JumpBlocker->OnComponentBeginOverlap.AddDynamic(this,&AVJPlayerController::BlockJump);
	}

	// OverlayWidgetController에서 DialogueManager에 바인딩해야하니까 그 전에 매니저 생성하기
	InitDialogueManager();

	if (AVJHUD* HUD = Cast<AVJHUD>(GetHUD()))
	{
		HUD->InitOverlay(this);
	}

	PlayerCharacter->OnActorDetected.AddUObject(this,&AVJPlayerController::OnActorDetected);
	PlayerCharacter->OnClearedInteractionActor.AddUObject(this,&AVJPlayerController::ClearInteraction);
		
}

void AVJPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateHighlightStates();
}

void AVJPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2d InputAxisVector = InputActionValue.Get<FVector2D>();
	if (PlayerCharacter.IsValid())
	{
		PlayerCharacter->HandleMovementInput(InputAxisVector);
	}
}

void AVJPlayerController::MoveCamera(const FInputActionValue& InputActionValue)
{
	const FVector2d InputAxisVector = InputActionValue.Get<FVector2D>();

	if (PlayerCharacter.IsValid())
	{
		PlayerCharacter->HandleLookInput(InputAxisVector);
	}
}


void AVJPlayerController::ChangeVerse(const FInputActionValue& InputActionValue)
{
	// Modifier가 먼저 눌러져있는 상태에서만 동작하도록
	if (!PlayerCharacter.IsValid()) return;
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
	if (PlayerCharacter.IsValid())
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
	if (PlayerCharacter.IsValid())
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
	if (!PlayerCharacter.IsValid()) return;
	// Modifier가 먼저 눌러져있는 상태에서만 동작하도록
	if (PlayerCharacter->IsModifierPressed() == false) return;
	
	UVerseStateSubsystem* VerseStateSubsystem = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UVerseStateSubsystem>();
	checkf(VerseStateSubsystem,TEXT("VerseStateSubsystem was NULL when tries to ChangeVerse"));
	VerseStateSubsystem->MoveToTargetState();
}

void AVJPlayerController::ModifierPressed()
{
	if (PlayerCharacter.IsValid())
	{
		PlayerCharacter->PressModifier();	
	}
}

void AVJPlayerController::ModifierReleased()
{
	if (PlayerCharacter.IsValid())
	{
		PlayerCharacter->ReleaseModifier();	
	}
}

void AVJPlayerController::Interact()
{
	if (PlayerCharacter.IsValid() && CurrentInteractionComponent.IsValid())
	{
		// TODO: 결과를 bool로 리턴 받아서 UI 피드백에 사용해도 될 것 같음
		CurrentInteractionComponent->TryInteract(this);
	}
}

void AVJPlayerController::AdvanceDial()
{
	// 현재 대화 진행 중인 상태에서 상호작용 버튼은 다음 대사로 넘기기
	if (DialogueManager && DialogueManager->IsActive())
	{
		DialogueManager->AdvanceDialogue();
	}
}

void AVJPlayerController::BlockJump(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor != GetPawn())
	{
		StopJump();
	}
}

void AVJPlayerController::SwapIMC(UInputMappingContext* NewIMC) const
{
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem && NewIMC)
	{
		Subsystem->ClearAllMappings();
		Subsystem->AddMappingContext(NewIMC,0);
	}
}

void AVJPlayerController::UpdateHighlightStates()
{
	if (PlayerCharacter.IsValid())
	{
		TSet<TWeakObjectPtr<AActor>> CurrentHighlightCandidates;
		PlayerCharacter->GetFilteredHighlightCandidates(CurrentHighlightCandidates);
		// 1. 현재 범위 내의 액터
		for (const TWeakObjectPtr<AActor>& Candidate : CurrentHighlightCandidates)
		{
			if (!Candidate.IsValid()) continue;
			// 이미 인터페이스 검증은 되어있으므로 바로 호출해도 됨

			IHighlightInterface* HighlightInterface = Cast<IHighlightInterface>(Candidate);
			const bool IsHighlighted = HighlightInterface->IsHighlighted();

			// 1-1. 이미 On : skip
			if (IsHighlighted) continue;
			// 1-2. Off 상태 : Highlight
			HighlightInterface->HighlightActor();
		}
		// 2. 이전 프레임에는 있었지만 현재는 없는 액터 : Invoker 범위를 벗어났으므로 UnHighlight
		for (const TWeakObjectPtr<AActor>& Candidate : LastHighlightCandidates)
		{
			if (!Candidate.IsValid()) continue;
			
			if (!CurrentHighlightCandidates.Contains(Candidate))
			{
				IHighlightInterface* HighlightInterface = Cast<IHighlightInterface>(Candidate);
				const bool IsHighlighted = HighlightInterface->IsHighlighted();
				// 2-1. 이미 off : skip
				if (!IsHighlighted) continue;
				// 2-2. On 상태 : UnHighlight
				HighlightInterface->UnHighlightActor();
			}
		}
		// 액터 갱신
		LastHighlightCandidates = MoveTemp(CurrentHighlightCandidates);
	}
}

void AVJPlayerController::OnActorDetected(AActor* NewActor)
{
	///TODO: 위젯에게 전달해줌
	if (NewActor == nullptr) return;

	if (UInteractionComponent* IC = NewActor->GetComponentByClass<UInteractionComponent>())
	{
		CurrentInteractionComponent = IC;
		// Widget Controller 에서 IC 컨트롤할 수 있도록 컴포넌트 자체 넘겨줌
		OnInteractableActorDetected.Broadcast(IC);
		// Highlight Interface를 구현한 액터라면 Color 변경해주기
		if (IHighlightInterface* HighlightInterface = Cast<IHighlightInterface>(NewActor))
		{
			HighlightInterface->SetHighlightColor(CUSTOM_DEPTH_INTERACTION);
		}
	}
	else
	{
		// 기존엔 있었지만 새로 찾은 액터에는 없을 때
		if (CurrentInteractionComponent.IsValid())
		{
			ClearInteraction(); 
		}
	}
	///Actor Valid Check, Actor Has Interaction Component Check
	///현재 상호작용 가능한 액터를 갱신
	///상호작용은 상호작용 버튼을 눌렀을 때 해당 컴포넌트의 액션을 호출
}

void AVJPlayerController::ClearInteraction()
{
	// 정리할 내용이 없으면 얼리 리턴
	
	///위젯 비저빌리티 끄기, 메타데이터 초기화
	///Highlight Interface를 구현한 액터라면 Color 초기화
	///외부에서 삭제되는 경우 CurrentIC가 nullptr이 될 수 있으므로 얼리 리턴 대신 조건문으로 래핑함 
	if (CurrentInteractionComponent.IsValid())
	{
		if (IHighlightInterface* HighlightInterface = Cast<IHighlightInterface>(CurrentInteractionComponent->GetOwner()))
		{
			HighlightInterface->SetHighlightColor(CUSTOM_DEPTH_DEFAULT);
		}
	}
	CurrentInteractionComponent = nullptr;
	OnInteractableActorLost.Broadcast();

}

void AVJPlayerController::InitDialogueManager()
{
	checkf(DialogueManagerClass,TEXT("DialogueManager Is Null"));
	DialogueManager = NewObject<UDialogueManager>(this,DialogueManagerClass);
	DialogueManager->OnDialogueStart.AddUObject(this,&AVJPlayerController::HandleDialogueStart);
	DialogueManager->OnDialogueEnd.AddUObject(this,&AVJPlayerController::HandleDialogueEnd);
}

void AVJPlayerController::HandleDialogueStart()
{
	// 대화 시작하면 다른 입력 막기위해 IMC 스왑
	SwapIMC(DialContext);
}

void AVJPlayerController::HandleDialogueEnd()
{
	// 대화 끝나면 원래대로 스왑
	SwapIMC(VJContext);
}
