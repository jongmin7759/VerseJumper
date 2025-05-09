// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/VJWidgetController.h"

void UVJWidgetController::SetWidgetControllerParams(APlayerController* InPlayerController)
{
	this->PlayerController = InPlayerController;
	
}

void UVJWidgetController::BroadcastInitialValues()
{
}

void UVJWidgetController::BindCallbacksToDependencies()
{
}
