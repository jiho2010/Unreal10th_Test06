// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/HUDWidget.h"
#include "Widget/PlayerStatWidget.h"

void UHUDWidget::InitializeWithAbilitySystem(AActor* InActor)
{
	if (PlayerStatWidget)
	{
		PlayerStatWidget->InitializeWithAbilitySystem(InActor);
	}
}
