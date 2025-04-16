// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "Interface/VerseStateInterface.h"
#include "VerseStateResponsiveActor.generated.h"

UCLASS()
class VERSEJUMPER_API AVerseStateResponsiveActor : public AActor , public IVerseStateInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVerseStateResponsiveActor();

	UFUNCTION(BlueprintCallable)
	FGameplayTag GetCurrentState() const {return CurrentState;}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void Internal_OnVerseStateChanged(const FGameplayTag& NewState) override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnVerseStateChanged(const FGameplayTag& NewState);

private:
	FGameplayTag CurrentState = FGameplayTag();
};
