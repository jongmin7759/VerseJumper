// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/VJCharacterBase.h"

// Sets default values
AVJCharacterBase::AVJCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AVJCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVJCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AVJCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

