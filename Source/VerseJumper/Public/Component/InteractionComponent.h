// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTryInteractSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractSignature, APlayerController* ,InstigatorController);

USTRUCT(BlueprintType)
struct FICMetaData
{
	GENERATED_BODY()
	FICMetaData(){}
	FICMetaData(bool InCanInteract, FText InActorName, FText InActionName) :
		bCanInteract(InCanInteract), ActorName(InActorName), ActionName(InActionName){}
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bCanInteract = true;
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FText ActorName;
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FText ActionName;
};


// 상호작용에 관련한 기능
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VERSEJUMPER_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInteractionComponent();

	bool TryInteract(APlayerController* InstigatorController);

	UPROPERTY(BlueprintAssignable)
	FOnTryInteractSignature OnTryInteract;
	UFUNCTION(BlueprintCallable)
	void SetCanInteract(bool NewSetting) {bCanInteract = NewSetting;}
	// 현재 IC 메타데이터를 구조체로 래핑해서 전달
	UFUNCTION(BlueprintCallable)
	FICMetaData GetMetaData() const;
	
	UPROPERTY(BlueprintAssignable)
	FOnInteractSignature OnInteractSuccess;
	UPROPERTY(BlueprintAssignable)
	FOnInteractSignature OnInteractFailed;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Interaction|MetaData")
	bool bCanInteract = true;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Interaction|MetaData")
	FText ActorName;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Interaction|MetaData")
	FText ActionName;
};
