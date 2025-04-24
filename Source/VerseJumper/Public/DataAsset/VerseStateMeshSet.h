// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "VerseStateMeshSet.generated.h"

struct FActorTypeRow;

USTRUCT(BlueprintType)
struct FVerseStateMeshEntry
{
	GENERATED_BODY()

	// DataTable 기준 Type
	// TODO : GetVerseStateTypes 함수를 헬퍼 펑션으로 빼서 다른 곳에서 사용할 수 있도록 하기
	UPROPERTY(EditAnywhere,meta=(GetOptions="VerseStateResponsiveActor.GetVerseStateTypes"))
	FName Type; 

	UPROPERTY(EditAnywhere)
	FGameplayTag VerseState;

	UPROPERTY(EditAnywhere)
	UStaticMesh* Mesh = nullptr;

};

UCLASS()
class VERSEJUMPER_API UVerseStateMeshSet : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	TArray<FVerseStateMeshEntry> Entries;

	// 특정 타입+스테이트 조합에 대응하는 메시 가져오기
	UStaticMesh* GetMeshFor(FName Type, FGameplayTag State) const;
};
