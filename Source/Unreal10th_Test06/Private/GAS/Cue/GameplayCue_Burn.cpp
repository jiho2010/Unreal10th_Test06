// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Cue/GameplayCue_Burn.h"

#include "GameplayEffectTypes.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

AGameplayCue_Burn::AGameplayCue_Burn()
{
	bAutoDestroyOnRemove = true;

	bUniqueInstancePerInstigator = false;
	bUniqueInstancePerSourceObject = false;
}

bool AGameplayCue_Burn::OnActive_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters)
{
	return StartBurnEffect(MyTarget, Parameters);
}

bool AGameplayCue_Burn::WhileActive_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters)
{
	return StartBurnEffect(MyTarget, Parameters);
}

bool AGameplayCue_Burn::OnRemove_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters)
{
	if (IsValid(BurnNiagaraComponent))
	{
		BurnNiagaraComponent->DestroyComponent();
	}

	BurnNiagaraComponent = nullptr;

	return true;
}

bool AGameplayCue_Burn::StartBurnEffect(AActor* Target, const FGameplayCueParameters& Parameters)
{
	if (IsValid(BurnNiagaraComponent))
	{
		return true;
	}

	if (!IsValid(Target) || !Target->GetRootComponent() || !BurnNiagaraSystem)
	{
		return false;
	}

	FVector EffectLocation = Target->GetActorLocation();

	if (const FHitResult* Hit = Parameters.EffectContext.GetHitResult())
	{
		EffectLocation = Hit->ImpactPoint;
	}

	BurnNiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
			BurnNiagaraSystem,
			Target->GetRootComponent(),
			NAME_None,
			EffectLocation,
			FRotator::ZeroRotator,
			EAttachLocation::KeepWorldPosition,
			false,
			true,
			ENCPoolMethod::None,
			false);

	return IsValid(BurnNiagaraComponent);
}