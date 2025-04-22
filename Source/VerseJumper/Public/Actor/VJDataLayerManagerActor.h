// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/VerseStateInterface.h"
#include "GameplayTagContainer.h"

#include "VJDataLayerManagerActor.generated.h"

UCLASS()
class VERSEJUMPER_API AVJDataLayerManagerActor : public AActor , public IVerseStateInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVJDataLayerManagerActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Internal_OnVerseStateChanged(const FGameplayTag& NewState) override;

	UPROPERTY(EditAnywhere, Category = "Mapping")
	TMap<FGameplayTag,const UDataLayerAsset*> StateLayerMap;

private:
	const UDataLayerAsset* CurrentDataLayer = nullptr;

};
