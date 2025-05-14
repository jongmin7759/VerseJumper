// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAsset/SurfaceSoundMap.h"

USoundBase* USurfaceSoundMap::GetFootstepSound(TEnumAsByte<EPhysicalSurface> SurfaceType) const
{
	if (FootstepSounds.Contains(SurfaceType))
	{
		return FootstepSounds[SurfaceType];
	}

	return nullptr;
}
