// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/OverHeadWidget.h"

#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "GAS/EnemyAttributeSet.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

void UOverHeadWidget::InitializeWithAbilitySystem(AActor* InActor)
{
	UnbindAttributeDelegates();

	SourceActor = InActor;

	if (IAbilitySystemInterface* Interface = Cast<IAbilitySystemInterface>(InActor))
	{
		ASC = Interface->GetAbilitySystemComponent();
	}

	if (!ASC.IsValid() || !ASC->GetSet<UEnemyAttributeSet>())
	{
		ASC.Reset();
		RefreshUI();
		return;
	}

	ASC->GetGameplayAttributeValueChangeDelegate(UEnemyAttributeSet::GetHealthAttribute()).AddUObject(this, &UOverHeadWidget::OnAttributeChanged);
	ASC->GetGameplayAttributeValueChangeDelegate(UEnemyAttributeSet::GetMaxHealthAttribute()).AddUObject(this, &UOverHeadWidget::OnAttributeChanged);

	RefreshUI();
}

void UOverHeadWidget::NativeConstruct()
{
	Super::NativeConstruct();

	InitializeWithAbilitySystem(SourceActor.Get());
}

void UOverHeadWidget::NativeDestruct()
{
	UnbindAttributeDelegates();

	Super::NativeDestruct();
}

void UOverHeadWidget::OnAttributeChanged(const FOnAttributeChangeData & Data)
{
	RefreshUI();
}

void UOverHeadWidget::RefreshUI()
{
	const UEnemyAttributeSet* Attributes = ASC.IsValid() ? ASC->GetSet<UEnemyAttributeSet>() : nullptr;

	const float Health = Attributes ? Attributes->GetHealth() : 0.0f;
	const float MaxHealth = Attributes ? Attributes->GetMaxHealth() : 0.0f;

	const float Percent = MaxHealth > 0.0f ? FMath::Clamp(Health / MaxHealth, 0.0f, 1.0f) : 0.0f;

	if (HealthText)
	{
		HealthText->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), Health, MaxHealth)));
	}

	if (HealthProgressBar)
	{
		HealthProgressBar->SetPercent(Percent);
	}
}

void UOverHeadWidget::UnbindAttributeDelegates()
{
	if (ASC.IsValid())
	{
		ASC->GetGameplayAttributeValueChangeDelegate(UEnemyAttributeSet::GetHealthAttribute()).RemoveAll(this);
		ASC->GetGameplayAttributeValueChangeDelegate(UEnemyAttributeSet::GetMaxHealthAttribute()).RemoveAll(this);
	}

	ASC.Reset();
}
