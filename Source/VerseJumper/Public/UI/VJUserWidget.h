// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VJUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class VERSEJUMPER_API UVJUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// 위젯 블루프린트에서 컨트롤러 셋할 수 있도록
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController);
	
	// 일방향 종속성을 위해 위젯이 컨트롤러를 포인터로 들고 컨트롤러는 위젯 안 들고 있음
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;

protected:
	// 컨트롤러가 셋 됐는지 확인하기위한 함수로 일종의 BeginPlay 역할 수행
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet();
};
