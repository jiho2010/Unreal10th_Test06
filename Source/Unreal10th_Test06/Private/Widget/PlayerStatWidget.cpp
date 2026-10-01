// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/PlayerStatWidget.h"

#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "GAS/PlayerAttributeSet.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

void UPlayerStatWidget::InitializeWithAbilitySystem(AActor* InActor)
{
	UnbindAttributeDelegates();

	SourceActor = InActor;

	if (IAbilitySystemInterface* Interface = Cast<IAbilitySystemInterface>(InActor))
	{
		ASC = Interface->GetAbilitySystemComponent();
	}

	if (!ASC.IsValid() || !ASC->GetSet<UPlayerAttributeSet>())
	{
		ASC.Reset();
		RefreshUI();
		return;
	}

	ASC->GetGameplayAttributeValueChangeDelegate(UPlayerAttributeSet::GetHealthAttribute()).AddUObject(this, &UPlayerStatWidget::OnAttributeChanged);
	ASC->GetGameplayAttributeValueChangeDelegate(UPlayerAttributeSet::GetMaxHealthAttribute()).AddUObject(this, &UPlayerStatWidget::OnAttributeChanged);
	ASC->GetGameplayAttributeValueChangeDelegate(UPlayerAttributeSet::GetManaAttribute()).AddUObject(this, &UPlayerStatWidget::OnAttributeChanged);
	ASC->GetGameplayAttributeValueChangeDelegate(UPlayerAttributeSet::GetMaxManaAttribute()).AddUObject(this, &UPlayerStatWidget::OnAttributeChanged);

	RefreshUI();
}

void UPlayerStatWidget::NativeConstruct()
{
	Super::NativeConstruct();

	InitializeWithAbilitySystem(SourceActor.Get());
}

void UPlayerStatWidget::NativeDestruct()
{
	UnbindAttributeDelegates();

	Super::NativeDestruct();
}

void UPlayerStatWidget::OnAttributeChanged(const FOnAttributeChangeData & Data)
{
	RefreshUI();
}

void UPlayerStatWidget::RefreshUI()
{
	const UPlayerAttributeSet* Attributes = ASC.IsValid() ? ASC->GetSet<UPlayerAttributeSet>() : nullptr;

	const float Health = Attributes ? Attributes->GetHealth() : 0.0f;
	const float MaxHealth = Attributes ? Attributes->GetMaxHealth() : 0.0f;

	const float Mana = Attributes ? Attributes->GetMana() : 0.0f;
	const float MaxMana = Attributes ? Attributes->GetMaxMana() : 0.0f;

	const float HealthPercent = MaxHealth > 0.0f ? FMath::Clamp(Health / MaxHealth, 0.0f, 1.0f) : 0.0f;
	const float ManaPercent = MaxMana > 0.0f ? FMath::Clamp(Mana / MaxMana, 0.0f, 1.0f) : 0.0f;

	if (HealthText)
	{
		HealthText->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), Health, MaxHealth)));
	}

	if (HealthProgressBar)
	{
		HealthProgressBar->SetPercent(HealthPercent);
	}

	if (ManaText)
	{
		ManaText->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), Mana, MaxMana)));
	}

	if (ManaProgressBar)
	{
		ManaProgressBar->SetPercent(ManaPercent);
	}
}

void UPlayerStatWidget::UnbindAttributeDelegates()
{
	if (ASC.IsValid())
	{
		ASC->GetGameplayAttributeValueChangeDelegate(UPlayerAttributeSet::GetHealthAttribute()).RemoveAll(this);
		ASC->GetGameplayAttributeValueChangeDelegate(UPlayerAttributeSet::GetMaxHealthAttribute()).RemoveAll(this);
		ASC->GetGameplayAttributeValueChangeDelegate(UPlayerAttributeSet::GetManaAttribute()).RemoveAll(this);
		ASC->GetGameplayAttributeValueChangeDelegate(UPlayerAttributeSet::GetMaxManaAttribute()).RemoveAll(this);
	}

	ASC.Reset();
}
