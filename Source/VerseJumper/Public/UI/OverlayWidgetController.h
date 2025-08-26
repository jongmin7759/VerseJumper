// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UI/VJWidgetController.h"
#include "OverlayWidgetController.generated.h"

struct FDialogueRow;
struct FICMetaData;
class UInteractionComponent;

// DataTable에 사용할 구조체 (TableRowBase 상속)
USTRUCT(BlueprintType)
struct FTutorialWidgetRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag MessageTag = FGameplayTag();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (MultiLine=true))
	FText MessageText = FText();
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStatusChangedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCreateTutorialSignature,FTutorialWidgetRow,TutorialRow);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteractFeedbackSignature, bool, bResult);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnVisibilityChangedSignature, bool, bShow);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnVerseStateChangedSignature,const FGameplayTag&,NewState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnICUpdateSignature,const FICMetaData&,NewICMetaData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialogueWidgetUpdatedSignature,const FDialogueRow&,NewDialogue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCollectibleNumUpdatedSignature, int32, NewNum, bool , Init);

UCLASS(BlueprintType, Blueprintable)
class VERSEJUMPER_API UOverlayWidgetController : public UVJWidgetController
{
	GENERATED_BODY()
public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable)
	FOnStatusChangedSignature OnJumpBegin;
	UPROPERTY(BlueprintAssignable)
	FOnStatusChangedSignature OnJumpEnd;
	UPROPERTY(BlueprintAssignable)
	FOnStatusChangedSignature OnModifierPressed;
	UPROPERTY(BlueprintAssignable)
	FOnStatusChangedSignature OnModifierReleased;
	UPROPERTY(BlueprintAssignable)
	FOnVerseStateChangedSignature OnVerseStateSet;
	UPROPERTY(BlueprintAssignable)
	FOnVerseStateChangedSignature OnTargetStateSet;
	UPROPERTY(BlueprintAssignable)
	FOnICUpdateSignature OnInteractableActorDetected;
	UPROPERTY(BlueprintAssignable)
	FOnStatusChangedSignature OnInteractableActorLost;
	UPROPERTY(BlueprintAssignable)
	FOnDialogueWidgetUpdatedSignature OnDialogueUpdated;
	UPROPERTY(BlueprintAssignable)
	FOnStatusChangedSignature OnDialogueStart;
	UPROPERTY(BlueprintAssignable)
	FOnStatusChangedSignature OnDialogueEnd;
	UPROPERTY(BlueprintAssignable)
	FOnCollectibleNumUpdatedSignature OnCollectibleNumUpdated;
	// Interact Feedback
	UPROPERTY(BlueprintAssignable)
	FInteractFeedbackSignature OnInteract;
	// Set Visibility
	UPROPERTY(BlueprintAssignable)
	FOnVisibilityChangedSignature OnVisibilityChanged;
	// Tutorial
	UPROPERTY(BlueprintAssignable)
	FOnStatusChangedSignature OnRemoveTutorial;
	UPROPERTY(BlueprintAssignable)
	FOnCreateTutorialSignature OnCreateTutorial;
	// Sequence
	UPROPERTY(BlueprintAssignable)
	FOnStatusChangedSignature OnSequencePlaying;
	UPROPERTY(BlueprintAssignable)
	FOnStatusChangedSignature OnSequenceStopped;
	
	protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Interaction")
	TObjectPtr<UInteractionComponent> CurrentInteractionComponent;

	// 컨트롤러를 통해 DT에 있는 정보 접근할 수 있도록 변수 추가
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Data")
	TObjectPtr<UDataTable> TutorialWidgetDataTable;

	// 태그로 임의의 테이블 로우 반환하도록 템플릿 함수 만들기 (TODO : 라이브러리 헤더로 옮겨서 Generic하게 사용할 수 있도록하기) 
	template<typename T>
	T* GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag);
};

template <typename T>
T* UOverlayWidgetController::GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag)
{
	if (DataTable == nullptr) return nullptr;
	// 행 이름을 태그 이름과 일치하게 만들었기 때문에 이렇게 찾을 수 있음
	return DataTable->FindRow<T>(Tag.GetTagName(),TEXT("")/*미사용*/);
}