// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "VJHUD.generated.h"

class UVJUserWidget;
class UOverlayWidgetController;
/**
 * 
 */
UCLASS()
class VERSEJUMPER_API AVJHUD : public AHUD
{
	GENERATED_BODY()

public:

	UPROPERTY()
	TObjectPtr<UVJUserWidget> OverlayWidget;

	// Controller도 같이 만들기 (Getter지만 없다면 하나 만들고 이미 있다면 있는 컨트롤러 리턴)
	UOverlayWidgetController* GetOverlayWidgetController(APlayerController* PlayerController);

	// Overlay 구성을 위한 위젯과 컨트롤러 생성
	void InitOverlay(APlayerController* PlayerController);

	
private:

	UPROPERTY(EditAnywhere)
	TSubclassOf<UVJUserWidget> OverlayWidgetClass;

	// 컨트롤러 만들기 위한 클래스와 만든 뒤의 포인터
	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;
	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;
};
