// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DialogueComponent.generated.h"

// 대화 시작만 담당하고 진행과 대사 테이블 참조는 매니저가
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VERSEJUMPER_API UDialogueComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UDialogueComponent();
	
	UFUNCTION(BlueprintCallable)
	void SetStartingLineID(FName NewStartingLineID);

	const FName& GetStartingLineID() const {return StartingLineID;} 

	UFUNCTION(BlueprintCallable)
	void BeginDialogue(APlayerController* InstigatorController , FName OverridingStartingLineID = FName("NotOverride"));
	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(AllowPrivateAccess=true), Category="Dialogue")
	FName StartingLineID;
		
};
