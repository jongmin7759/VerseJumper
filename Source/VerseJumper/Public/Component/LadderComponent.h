// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "LadderComponent.generated.h"

class UArrowComponent;
/**
 * 
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class VERSEJUMPER_API ULadderComponent : public UBoxComponent
{
	GENERATED_BODY()
public:	
	ULadderComponent();

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void EnterLadder(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	void ExitLadder(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY()
	TObjectPtr<UArrowComponent> ArrowComponent = nullptr;

};
