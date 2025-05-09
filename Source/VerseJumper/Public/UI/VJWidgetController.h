// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "VJWidgetController.generated.h"

/**
 * 
 */
UCLASS()
class VERSEJUMPER_API UVJWidgetController : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetControllerParams(APlayerController* InPlayerController);
	virtual void BroadcastInitialValues();
	// 컨트롤할 위젯들에 바인딩하기
	virtual void BindCallbacksToDependencies();
	
	// 자식 컨트롤러들이 접근할 수 있도록 protected
	// Model에 접근하기 위한 포인터들 (일방향 종속성 유지 View -> Controller -> Model) 
protected:
	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<APlayerController> PlayerController;
};
