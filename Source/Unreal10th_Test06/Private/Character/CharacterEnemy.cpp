// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterEnemy.h"
#include "Widget/OverHeadWidget.h"
#include "AbilitySystemComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GAS/EnemyAttributeSet.h"

ACharacterEnemy::ACharacterEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	OverHeadWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidgetComp"));
	OverHeadWidgetComponent->SetupAttachment(RootComponent);

	OverHeadWidgetComponent->SetWidgetSpace(EWidgetSpace::World);
	OverHeadWidgetComponent->SetDrawSize(FVector2D(150.0f, 20.0f));
	OverHeadWidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));
	OverHeadWidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	EnemyAttributeSet = CreateDefaultSubobject<UEnemyAttributeSet>(TEXT("EnemyAttributeSet"));
}

UEnemyAttributeSet* ACharacterEnemy::GetUEnemyAttribute() const
{
	return EnemyAttributeSet.Get();
}

void ACharacterEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(AbilitySystemComponent))
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		InitializeOverHeadWidget();
	}
}

void ACharacterEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bFaceCamera)
	{
		UpdateOverheadWidgetRotation();
	}
}

void ACharacterEnemy::InitializeOverHeadWidget()
{
	if (!OverHeadWidgetComponent) return;

	if (UUserWidget* UserWidget = OverHeadWidgetComponent->GetUserWidgetObject())
	{
		if (UOverHeadWidget* OverHeadWidget = Cast<UOverHeadWidget>(UserWidget))
		{
			OverHeadWidget->InitializeWithAbilitySystem(this);
		}
	}
}

void ACharacterEnemy::UpdateOverheadWidgetRotation()
{
	if (!OverHeadWidgetComponent) return;

	if (APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(this, 0))
	{
		const FVector CameraForward = CameraManager->GetCameraRotation().Vector();
		FRotator WidgetRotation = (-CameraForward).Rotation();

		if (bLockWidgetPitch)
		{
			WidgetRotation.Pitch = 0.0f;
		}
		if (bLockWidgetRoll)
		{
			WidgetRotation.Roll = 0.0f;
		}

		OverHeadWidgetComponent->SetWorldRotation(WidgetRotation);
	}
}
