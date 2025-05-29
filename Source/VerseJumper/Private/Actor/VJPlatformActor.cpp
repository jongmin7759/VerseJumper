// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/VJPlatformActor.h"

#include "Components/BoxComponent.h"

AVJPlatformActor::AVJPlatformActor()
{
	CollisionBox = CreateDefaultSubobject<UBoxComponent>("CollisionBox");
	RootComponent = CollisionBox;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMesh->SetupAttachment(RootComponent);
}

void AVJPlatformActor::Internal_OnVerseStateChanged(const FGameplayTag& NewState)
{
	Super::Internal_OnVerseStateChanged(NewState);

	UStaticMesh* NewMesh = GetCurrentStaticMesh();
	if (NewMesh == nullptr) return;
	
	// 메시를 변경하고 콜리전 박스 크기에 맞춰 사이즈 보정까지 해준다.
	StaticMesh->SetStaticMesh(NewMesh);
	const FVector BoxExtent = CollisionBox->GetScaledBoxExtent();
	const FVector MeshBounds = GetCurrentStaticMesh()->GetBounds().BoxExtent;
	const FVector ScaleToFit = BoxExtent / MeshBounds;
	const float MeshScale = GetCurrentMeshScale();
	// 메시 스케일 오프셋 적용
	StaticMesh->SetWorldScale3D(ScaleToFit*MeshScale);
}
