// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SurfaceSoundMap.generated.h"

// Footstep Component에서 사용하기 위한 DataAsset
// 지면과 사운드 매핑을 위해 사용
UCLASS()
class VERSEJUMPER_API USurfaceSoundMap : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	TMap<TEnumAsByte<EPhysicalSurface>, USoundBase*> FootstepSounds;

	USoundBase* GetFootstepSound(TEnumAsByte<EPhysicalSurface> SurfaceType) const;
};
