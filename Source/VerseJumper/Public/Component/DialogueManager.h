#pragma once

#include "CoreMinimal.h"
#include "Types/VJTypes.h"
#include "DialogueManager.generated.h"


DECLARE_DELEGATE_OneParam(FOnDialogueUpdatedSignature, const FDialogueRow&);
DECLARE_MULTICAST_DELEGATE(FOnDialogueStartSignature);
DECLARE_MULTICAST_DELEGATE(FOnDialogueEndSignature);

class UDialogueComponent;
/**
 * 대화 흐름 처리
 */
UCLASS(BlueprintType, Blueprintable)
class VERSEJUMPER_API UDialogueManager : public UObject
{
	GENERATED_BODY()

public:
	void StartDialogue(UDialogueComponent* DialogueComponent);
	void AdvanceDialogue();
	void EndDialogue();
	bool IsActive() const {return bIsActive;}

	// Delegate
	FOnDialogueStartSignature OnDialogueStart;
	FOnDialogueUpdatedSignature OnDialogueUpdated;
	FOnDialogueEndSignature OnDialogueEnd;
private:
	TWeakObjectPtr<UDialogueComponent> CurrentDialogueComponent;
	bool bIsActive = false;
	bool HasNextLine() const;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UDataTable> DialogueTable;
	const FDialogueRow* GetDialogueRowByCurrentLine() const;
	FName CurrentLineID;
	// Delegate 호출할 때 데이터 테이블에서는 raw ptr 리턴하니까 nullptr 리턴하면 임시객체 전달하도록 함수 분리
	void UpdateDialogue(const FDialogueRow* DialogueRow) const;
};
