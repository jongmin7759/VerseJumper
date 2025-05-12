// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OverlayWidgetController.h"

#include "Character/VJPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystem/VerseStateSubsystem.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	// 점프 버튼 위젯은 착지 상태가 초기 상태
	HandleLanding(FHitResult());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	// OnPossess 이후에 호출되기때문에 캐릭터는 할당되어있음
	AVJPlayerCharacter* PlayerCharacter = Cast<AVJPlayerCharacter>(PlayerController->GetCharacter());

	PlayerCharacter->OnJumped.AddLambda(
		[this]()
		{
			OnJumpBegin.Broadcast();
		}
	);
	PlayerCharacter->LandedDelegate.AddDynamic(this, &UOverlayWidgetController::HandleLanding);
	PlayerCharacter->OnModifierPressed.AddLambda(
		[this]()
		{
			OnModifierPressed.Broadcast();
		}
	);
	PlayerCharacter->OnModifierReleased.AddLambda(
		[this]()
		{
			OnModifierReleased.Broadcast();
		}
	);

	// Subsystem에서 바인딩
	UVerseStateSubsystem* VerseStateSubsystem = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UVerseStateSubsystem>();
	VerseStateSubsystem->VerseStateChanged.AddLambda(
		[this](const FGameplayTag& NewState)
		{
			OnVerseStateSet.Broadcast(NewState);
		}
	);
}

void UOverlayWidgetController::HandleLanding(const FHitResult& Hit)
{
	OnLanded.Broadcast();
}
