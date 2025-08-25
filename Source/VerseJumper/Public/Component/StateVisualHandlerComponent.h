// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "StateVisualHandlerComponent.generated.h"

class UVerseStateVisualMap;
/**
 * GameMode에 넣어서 스테이트 비주얼에 대해 관리해줄 컴포넌트
 * 1. 월드파티션의 레이어를 온/오프
 * 2. 랜드스케이프의 머티리얼 레이어, MPC 조정
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VERSEJUMPER_API UStateVisualHandlerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UStateVisualHandlerComponent();

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void PlayGlitchEffect();

protected:
	virtual void BeginPlay() override;


	UPROPERTY(EditAnywhere, Category="Visual Handler")
	TObjectPtr<UVerseStateVisualMap> VerseStateVisualMap = nullptr;

	UPROPERTY(EditAnywhere, Category="Visual Handler")
	TObjectPtr<UMaterialParameterCollection> LandscapeMPC;

	UPROPERTY(EditAnywhere, Category="Visual Handler")
	TObjectPtr<UMaterialParameterCollection> FXMPC;

	UPROPERTY(EditAnywhere, Category="Visual Handler")
	TObjectPtr<UCurveFloat> GlitchIntensityCurve = nullptr;

private:
	// 모든 레이어 로드해두기
	void InitDataLayers();
	void HandleStateChange(const FGameplayTag& NewState);
	void HandleWorldPartitionLayerVisibility(const FGameplayTag& NewState);
	void HandleMPC(const FGameplayTag& NewState);
	void PlayCurve();
	void UpdateFromCurve(float DeltaTime);
	const UDataLayerAsset* CurrentDataLayer = nullptr;

	float Playhead = 0.f;
	float EndTime = 0.f;
	bool  bPlaying = false;
};
