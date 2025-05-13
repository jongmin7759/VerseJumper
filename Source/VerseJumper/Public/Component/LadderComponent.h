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
	
	UFUNCTION(BlueprintPure)
	bool IsActorNearLadderTop(AActor* InActor) const;

	TEnumAsByte<EPhysicalSurface> GetLadderSurface() const {return PhysicalSurface;}
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Ladder")
	float LadderTopOffset = 30.f;
	
private:
	UFUNCTION()
	void EnterLadder(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	void ExitLadder(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	// Footstep Component에서 사용하기 위한 속성값
	UPROPERTY(EditAnywhere, Category="Ladder")
	TEnumAsByte<EPhysicalSurface> PhysicalSurface;
};
