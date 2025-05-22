// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/VerseStateResponsiveActor.h"
#include "VJLadderActor.generated.h"

class UBoxComponent;
/**
 * 
 */
UCLASS()
class VERSEJUMPER_API AVJLadderActor : public AVerseStateResponsiveActor
{
	GENERATED_BODY()
public:
	AVJLadderActor();
	
	UFUNCTION(BlueprintPure)
	bool IsActorNearLadderTop(AActor* InActor) const;

	TEnumAsByte<EPhysicalSurface> GetLadderSurface() const {return PhysicalSurface;}
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Platform Properties")
	TObjectPtr<UBoxComponent> CollisionBox;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Platform Properties")
	TObjectPtr<UStaticMeshComponent> StaticMesh;

	virtual void Internal_OnVerseStateChanged(const FGameplayTag& NewState) override;
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
