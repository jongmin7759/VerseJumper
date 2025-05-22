#include "Character/VJCharacterBase.h"

#include "Component/FootstepComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystem/VerseStateSubsystem.h"

AVJCharacterBase::AVJCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
	FootstepComponent = CreateDefaultSubobject<UFootstepComponent>("FootstepComponent");
}
void AVJCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
	// OnVerseStateChanged 바인딩
	UVerseStateSubsystem* VerseStateSubsystem = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UVerseStateSubsystem>();
	checkf(VerseStateSubsystem,TEXT("VerseStateSubsystem was NULL when tries to Bind OnVerseStateChanged"));
	VerseStateSubsystem->VerseStateChanged.AddUObject(this,&IVerseStateInterface::Internal_OnVerseStateChanged);
	
}

void AVJCharacterBase::Internal_OnVerseStateChanged(const FGameplayTag& NewState)
{
}

// Called every frame
void AVJCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AVJCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AVJCharacterBase::EnterLadder(AVJLadderActor* NewLadder)
{
	SetIsOnLadder(true);
	GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	GetCharacterMovement()->StopMovementImmediately();
	CurrentLadder = NewLadder;
}

void AVJCharacterBase::ExitLadder()
{
	SetIsOnLadder(false);
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	CurrentLadder = nullptr;
}

bool AVJCharacterBase::IsNearGround() const
{
	// 캡슐의 바닥을 기준으로 할 것
	const float BottomPos = GetActorLocation().Z -(GetCapsuleComponent()->GetScaledCapsuleHalfHeight()); 
	const FVector Start (GetActorLocation().X, GetActorLocation().Y, BottomPos);
	
	const FVector End = Start - FVector(0.f, 0.f, GroundDetectionDistance); // 캐릭터 아래 방향으로 50cm

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	const bool bHit = GetWorld()->LineTraceSingleByChannel(
		Hit,Start,End,ECC_Visibility,Params
	);

	return bHit;
}



