// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/VerseStateResponsiveActor.h"
#include "VJPlatformActor.generated.h"

class UBoxComponent;
/**
 * 플랫폼으로 사용할 액터이므로 콜리전 박스가 실질적인 액터의 본체
 * 메시는 변화하지만 콜리전 박스의 범위를 크게 벗어나서는 안 된다.
 * 메시의 크기 보정이 필요.
 */
UCLASS()
class VERSEJUMPER_API AVJPlatformActor : public AVerseStateResponsiveActor
{
	GENERATED_BODY()

public:
	AVJPlatformActor();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Platform Properties")
	TObjectPtr<UBoxComponent> CollisionBox;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Platform Properties")
	TObjectPtr<UStaticMeshComponent> StaticMesh;

	virtual void Internal_OnVerseStateChanged(const FGameplayTag& NewState) override;

};
