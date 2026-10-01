// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/GameHUD.h"
#include "Widget/HUDWidget.h"
#include "Blueprint/UserWidget.h"

void AGameHUD::InitHUD(APawn* InPawn)
{
	if (!IsValid(InPawn)) return;

	APlayerController* PC = GetOwningPlayerController();

	if (!IsValid(PC) || !PC->IsLocalController()) return;

	if (!IsValid(HUDWidget))
	{
		if (!HUDWidgetClass) return;

		HUDWidget = CreateWidget<UHUDWidget>(PC, HUDWidgetClass);
	}

	if (!IsValid(HUDWidget)) return;

	if (!HUDWidget->IsInViewport())
	{
		HUDWidget->AddToViewport();
	}
	HUDWidget->InitializeWithAbilitySystem(InPawn);
}

void AGameHUD::BeginPlay()
{
	Super::BeginPlay();

	if (APawn* OwningPawn = GetOwningPawn())
	{
		InitHUD(OwningPawn);
	}
}
