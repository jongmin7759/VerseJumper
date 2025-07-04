// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/HighlightInterface.h"
#include "VJInteractableActor.generated.h"

class UInteractionComponent;
// 
// Verse State와 무관하게 단순 상호작용을 위한 액터
// Highlight를 위해 인터페이스 구현
//
UCLASS()
class VERSEJUMPER_API AVJInteractableActor : public AActor , public IHighlightInterface
{
	GENERATED_BODY()
	
public:	
	AVJInteractableActor();

	// IHighlightInterface 
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	virtual bool IsHighlighted() const override;
	virtual void SetHighlightColor(int32 Value) override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnHighlight();
	UFUNCTION(BlueprintImplementableEvent)
	void OnUnHighlight();
	UFUNCTION(BlueprintImplementableEvent)
	void OnHighlightColorChanged(int32 Value);
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	UStaticMeshComponent* StaticMesh;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	UInteractionComponent* InteractionComponent;

private:
	bool bIsHighlighted = false;

	void InitHighlightColor();
};
