// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/VJUserWidget.h"

void UVJUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}
