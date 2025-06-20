// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MoverComponent.generated.h"

///
/// 플랫폼 이동을 처리하기위한 컴포넌트로 데이터 레이어 온/오프에 대응해서 BeginPlay에서 이동 오프셋 적용
/// 
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VERSEJUMPER_API UMoverComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UMoverComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void SetTargetLocation(FVector NewTargetLocation);
	UFUNCTION(BlueprintCallable)
	void StartMoving();
	UFUNCTION(BlueprintCallable)
	void StopMoving();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Mover")
	bool bAutoPingPong = false;
	UPROPERTY(EditAnywhere, Category = "Mover")
	float Speed = 200.0f;
	UPROPERTY(EditAnywhere, Category = "Mover")
	float PauseDuration = 1.0f;
	UPROPERTY(EditAnywhere, Category = "Mover")
	bool bAutoStart = true;

private:
	FVector StartLocation;
	FVector TargetLocation;
	FVector CurrentTargetLocation;

	bool bIsMoving = false;
	bool bForward = true;
	// TargetLocation fallback
	bool bTargetExplicitlySet = false;
	void SwapDirection();
};
