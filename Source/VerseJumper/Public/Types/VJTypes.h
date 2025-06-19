// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "VJTypes.generated.h"

// VerseStateResponsiveActor가 어떤 타입인지 정의
USTRUCT(BlueprintType)
struct FActorTypeRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName Type;
};

// 대사 테이블 행
USTRUCT(BlueprintType)
struct FDialogueRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Speaker = FText::FromString("None");

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Content = FText::FromString("None");

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName NextLine = FName();
};