// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/VJHUD.h"

#include "UI/VJUserWidget.h"
#include "UI/OverlayWidgetController.h"

UOverlayWidgetController* AVJHUD::GetOverlayWidgetController(APlayerController* PlayerController)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this,OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(PlayerController);
		OverlayWidgetController->BindCallbacksToDependencies();
	}
	return OverlayWidgetController;
}

void AVJHUD::InitOverlay(APlayerController* PlayerController)
{
	// 단순 check - crash 보다 상세하게 메시지 전달
	checkf(OverlayWidgetClass, TEXT("OverlaywidgetClass Uninitialized, Check BP_VJHUD"));
	checkf(OverlayWidgetControllerClass, TEXT("OverlayWidgetControllerClass Uninitialized, Check BP_VJHUD"));

	
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(),OverlayWidgetClass);
	OverlayWidget = Cast<UVJUserWidget>(Widget);
	
	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(PlayerController);

	OverlayWidget->SetWidgetController(OverlayWidgetController);
	WidgetController->BroadcastInitialValues();
	
	Widget->AddToViewport();
	
}