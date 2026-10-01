// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/GameplayAbility_Fireball.h"

#include "Projectile/FireballProjectile.h"

#include "Engine/World.h"
#include "GameFramework/Pawn.h"

UGameplayAbility_Fireball::UGameplayAbility_Fireball()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly;
}

void UGameplayAbility_Fireball::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo * ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData * TriggerEventData)
{
	APawn* PlayerPawn = Cast<APawn>(GetAvatarActorFromActorInfo());

	UWorld* World = PlayerPawn ? PlayerPawn->GetWorld() : nullptr;

	if (!IsValid(PlayerPawn) || !World || !ProjectileClass || !CostGameplayEffectClass || !CooldownGameplayEffectClass)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	const FVector Forward = PlayerPawn->GetActorForwardVector();
	const FVector SpawnLocation = PlayerPawn->GetActorLocation() + Forward * SpawnDistance + FVector(0.0f, 0.0f, SpawnHeightOffset);
	const FRotator SpawnRotation = Forward.Rotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = PlayerPawn;
	SpawnParams.Instigator = PlayerPawn;

	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AFireballProjectile* Projectile = World->SpawnActor<AFireballProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);

	EndAbility(Handle, ActorInfo, ActivationInfo, true, !IsValid(Projectile));
}
