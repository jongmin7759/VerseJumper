// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LifeCycleSaveComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSaveLoadSignature);

// WorldPartition 사용하고 있어서 액터의 세이브 로드 시점이 제각각이라
// 저장이 필요한 액터만 생명주기에 맞춰서 세이브 로드 할 수 있도록 컴포넌트 이용
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VERSEJUMPER_API ULifeCycleSaveComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULifeCycleSaveComponent();

	UFUNCTION(BlueprintCallable)
	void SaveOwnerActor();
	UPROPERTY(BlueprintAssignable)
	FOnSaveLoadSignature OnSaved;
	UPROPERTY(BlueprintAssignable)
	FOnSaveLoadSignature OnLoaded;

protected:
	// 여기서 로드
	virtual void BeginPlay() override;
	// 여기서 세이브
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	void LoadOwnerActor();
	bool bNeedSave = false;
};
