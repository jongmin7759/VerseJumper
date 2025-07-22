// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "Interface/HighlightInterface.h"
#include "VerseJumper/VerseJumper.h"
#include "CheckPoint.generated.h"

class ULifeCycleSaveComponent;
class USphereComponent;
/**
 * 
 */
UCLASS()
class VERSEJUMPER_API ACheckPoint : public APlayerStart , public IHighlightInterface
{
	GENERATED_BODY()
public:
	ACheckPoint(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintReadWrite, SaveGame)
	bool bReached = false;

	UFUNCTION(BlueprintNativeEvent)
	void CheckPointReached(bool ShouldSkipEffect = false);

protected:
	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void BeginPlay() override;

	// SaveLoad
	UFUNCTION()
	void OnLoaded();

	// IHighlightInterface 
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	virtual bool IsHighlighted() const override;
	virtual void SetHighlightColor(int32 Value) override;
	bool bIsHighlighted = false;

	UPROPERTY(EditDefaultsOnly)
	int32 CustomDepthStencilOverride = CUSTOM_DEPTH_INTERACTION;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> CheckpointMesh;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ULifeCycleSaveComponent> SaveComponent;
};
