// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Character/VJCharacterBase.h"
#include "VJPlayerCharacter.generated.h"

class UPlayerVerseStateComponent;
class UCollectibleTrackerComponent;
class USphereComponent;

DECLARE_MULTICAST_DELEGATE(FOnActionSignature);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnActorDetecedSignature,AActor*);
DECLARE_MULTICAST_DELEGATE_OneParam(FTutorialMessageRowSignature, const FGameplayTag&);

UCLASS()
class VERSEJUMPER_API AVJPlayerCharacter : public AVJCharacterBase
{
	GENERATED_BODY()
public:
	AVJPlayerCharacter();
	virtual void Tick(float DeltaSeconds) override;

	// Stat
	UFUNCTION(BlueprintCallable)
	void SetHasModifier(bool NewState) {bHasModifier = NewState;}
	UFUNCTION(BlueprintPure)
	bool HasModifier() const {return bHasModifier;}
	UFUNCTION(BlueprintCallable)
	void SetHasCardKey(bool NewState) {bHasCardKey = NewState;}
	UFUNCTION(BlueprintPure)
	bool HasCardKey() const {return bHasCardKey;}

	// Input
	USphereComponent* GetJumpBlocker() const {return JumpBlocker;}
	bool IsModifierPressed() const {return bIsModifierPressed;}
	void HandleMovementInput(const FVector2D& Input);
	void HandleLookInput(const FVector2D& Input);
	FOnActionSignature OnJumpBegin;
	//// 점프가 실행됐을 때 (위젯 컨트롤러에 브로드캐스팅하기위해 오버라이드)
	virtual void OnJumped_Implementation() override;
	FOnActionSignature OnJumpEnd;
	virtual void EnterLadder(AVJLadderActor* NewLadder) override;
	virtual void Landed(const FHitResult& Hit) override;
	FOnActionSignature OnModifierPressed;
	UFUNCTION()
	void PressModifier();
	FOnActionSignature OnModifierReleased;
	UFUNCTION()
	void  ReleaseModifier();
	// 착지 사운드 스패밍 방지용으로 낙하 시간 카운트
	void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode = 0) override;

	// VerseState
	UPlayerVerseStateComponent* GetPlayerVerseStateComponent() const {return PlayerVerseStateComponent;}
	void SetTargetStateToNext();
	void SetTargetStateToPrev();
	void VerseJumpToTarget();
	// Collectible
	UCollectibleTrackerComponent* GetCollectibleTracker() const {return CollectibleTracker;}

	// Highlight
	//// 시야 체크해서 넘겨줌
	void GetFilteredHighlightCandidates(TSet<TWeakObjectPtr<AActor>>& OutCandidates) const;

	// Interaction
	FOnActorDetecedSignature OnActorDetected;
	FOnActionSignature OnClearedInteractionActor;

	// SaveTag
	//// 컷신 재생 여부 등 저장하기 위한 태그 컨테이너
	FGameplayTagContainer SavedTags;


	// Tutorial 표시,제거
	//// TODO : Tutorial Manager 등으로 분리하는 것이 좋을듯
	UFUNCTION(BlueprintCallable)
	void ShowTutorial(const FGameplayTag& TutorialTag);
	UFUNCTION(BlueprintCallable)
	void RemoveTutorial();
	FTutorialMessageRowSignature TutorialMessageRow;
	FOnActionSignature OnRemoveTutorial;

	// Save&Load
	UFUNCTION(BlueprintCallable)
	void SavePlayerProgress(const FName& PlayerStartTag);
	UFUNCTION(BlueprintCallable)
	void LoadPlayerProgress();
	
protected:
	virtual void BeginPlay() override;
	// 점프 가능한 상황인지 판단 , ACharacter의 함수 오버라이드
	virtual bool CanJumpInternal_Implementation() const override; 
	
	// Jump할 때 머리가 닿으면 멈추도록하기 위해 머리에 붙일 콜리전 추가
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly , Category = "Player|Collision")
	TObjectPtr<USphereComponent> JumpBlocker;

	// Collectible Tracker
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Player|Collectible")
	TObjectPtr<UCollectibleTrackerComponent> CollectibleTracker;

	// SFX
	UPROPERTY(EditAnywhere, Category="Player|SFX")
	TObjectPtr<USoundBase> JumpSound;
	UPROPERTY(EditAnywhere, Category="Player|SFX")
	TObjectPtr<USoundBase> LandingSound;
	UPROPERTY(EditAnywhere, Category="Player|SFX")
	TObjectPtr<USoundBase> VerseJumpSound;
	UPROPERTY(EditAnywhere, Category="Player|SFX")
	TObjectPtr<USoundBase> TargetChangeSound;
	UPROPERTY(EditAnywhere, Category="Player|SFX")
	TObjectPtr<USoundBase> ModifierOnSound;
	UPROPERTY(EditAnywhere, Category="Player|SFX")
	TObjectPtr<USoundBase> ModifierOffSound;

	// IVerseStateInterface
	virtual void Internal_OnVerseStateChanged(const FGameplayTag& NewState) override;

	// Highlight Invoker Sphere (범위 안에 들어오면 외곽선 그릴 후보 액터로 추가)
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Player|Highlight")
	TObjectPtr<USphereComponent> HighlightInvokerSphere;

private:
	// Stat
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Player|Stat",meta=(AllowPrivateAccess=true))
	bool bHasModifier = false;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Player|Stat",meta=(AllowPrivateAccess=true))
	bool bHasCardKey = false;
	
	// Input
	bool bIsModifierPressed = false;
	
	// VerseStateComponent
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,meta=(AllowPrivateAccess=true),Category="VerseState")
	TObjectPtr<UPlayerVerseStateComponent> PlayerVerseStateComponent;

	// SFX
	void PlaySFX(USoundBase* SoundBase) const;

	// Highlight
	//// 이 시야각 안에 들어와야 하이라이트
	UPROPERTY(EditAnywhere, Category="Highlight")
	float HighlightAngle = 75.f;
	UPROPERTY(EditAnywhere, Category="Highlight")
	float HighlightRadius = 2000.f;
	//// 시야 체크
	bool IsInViewAngle(AActor* Target,float Angle) const;
	//// 외곽선 후보 액터
	TSet<TWeakObjectPtr<AActor>> HighlightCandidates;
	UFUNCTION()
	void AddHighlightCandidate(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	void RemoveHighlightCandidate(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	void RefreshHighlightCandidates();
	FTimerHandle RefreshTimerHandle;
	bool bUpdatingHighlightCandidates = false;
	TArray<TWeakObjectPtr<AActor>> PendingHighlightRemovals;

	// Interaction
	UPROPERTY(EditAnywhere, Category="Interaction")
	float InteractionDistance = 150.f;
	UPROPERTY(EditAnywhere, Category="Interaction")
	float InteractionRadius = 30.f;
	//// Actor로 캐싱해서 같은 액터 찍고있는 동안은 FindComponent 계속 하지 않도록
	TWeakObjectPtr<AActor> LastInteractingActor;
	TWeakObjectPtr<AActor> CurrentInteractingActor;
	void InteractionTrace();
	//// 카메라의 위치를 컴포넌트로 새롭게 구하는 것보다 높이 오프셋만 미리 구해서 저장해두기
	//// 카메라 컴포넌트를 블루프린트에서 추가했기때문에 FindComponent 써야하는데 틱마다 부르기 아까워서
	//// 그리고 어차피 근거리에서 상호작용 할 거니까 카메라가 혹시 조금씩 움직이더라도 정교하게 시야 맞출 필요 없을 거 같음
	float CameraHeightOffset = 40.f;
	// 외부 상황에서 InteractingActor가 삭제되는 경우
	// Current , Last 동시에 nullptr 되므로 적절하게 브로드캐스팅 안되는 문제 방지용 플래그
	bool bForceClearReady = false;

	float LastFallingTime = 0.f;
	float AirTimeThreshold = 0.3f;

};


