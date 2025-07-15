// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Trigger/TutorialTrigger.h"

#include "Character/VJPlayerCharacter.h"
#include "Components/ShapeComponent.h"
#include "Components/SphereComponent.h"

void ATutorialTrigger::BeginPlay()
{
	Super::BeginPlay();

	GetCollisionComponent()->OnComponentBeginOverlap.AddDynamic(this,&ATutorialTrigger::ShowTutorial);
	GetCollisionComponent()->OnComponentEndOverlap.AddDynamic(this,&ATutorialTrigger::RemoveTutorial);

}
void ATutorialTrigger::ShowTutorial(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AVJPlayerCharacter* PlayerCharacter = Cast<AVJPlayerCharacter>(OtherActor);
	if (PlayerCharacter == nullptr) return;

	// JumpBlocker에 먼저 걸려서 바로 End Overlap 되는 경우 방지
	if (OtherComp == Cast<UPrimitiveComponent>(PlayerCharacter->GetJumpBlocker())) return;

	// 중복 생성 X
	if (PlayerCharacter->SavedTags.HasTag(TutorialTag))
	{
		return;
	}

	// 플레이어 캐릭터에게 튜토리얼 태그 전달 (튜토리얼 생성은 캐릭터 -> 오버레이 위젯 컨트롤러 -> 오버레이 위젯)
	PlayerCharacter->ShowTutorial(TutorialTag);
}

void ATutorialTrigger::RemoveTutorial(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AVJPlayerCharacter* PlayerCharacter = Cast<AVJPlayerCharacter>(OtherActor);
	if (PlayerCharacter == nullptr) return;

	// JumpBlocker에 먼저 걸려서 바로 End Overlap 되는 경우 방지
	if (OtherComp == Cast<UPrimitiveComponent>(PlayerCharacter->GetJumpBlocker())) return;

	PlayerCharacter->RemoveTutorial();
}

