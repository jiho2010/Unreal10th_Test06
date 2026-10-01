// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/FireballProjectile.h"

#include "Character/CharacterEnemy.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "GameplayTagContainer.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

AFireballProjectile::AFireballProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;
	SetReplicatingMovement(true);

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));

	SetRootComponent(Collision);

	Collision->InitSphereRadius(15.0f);
	Collision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Collision->SetCollisionObjectType(ECC_WorldDynamic);

	Collision->SetCollisionResponseToAllChannels(ECR_Block);
	Collision->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	Collision->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);

	Collision->SetNotifyRigidBodyCollision(true);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	Mesh->SetupAttachment(Collision);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));

	Movement->SetUpdatedComponent(Collision);

	Movement->InitialSpeed = 1500.0f;
	Movement->MaxSpeed = 1500.0f;
	Movement->ProjectileGravityScale = 0.0f;

	Movement->bRotationFollowsVelocity = true;
	Movement->bShouldBounce = false;
	Movement->bInitialVelocityInLocalSpace = true;

	Movement->Velocity = FVector(1.0f, 0.0f, 0.0f);

	InitialLifeSpan = 5.0f;
}

// Called when the game starts or when spawned
void AFireballProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	Collision->OnComponentHit.AddDynamic(this, &AFireballProjectile::OnHit);

	if (AActor* OwnerActor = GetOwner())
	{
		Collision->IgnoreActorWhenMoving(OwnerActor, true);
	}

	if (APawn* InstigatorPawn = GetInstigator())
	{
		Collision->IgnoreActorWhenMoving(InstigatorPawn, true);
	}
}

void AFireballProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!HasAuthority() || bHitProcessed) return;
	if (OtherActor == this || OtherActor == GetOwner() || OtherActor == GetInstigator()) return;

	bHitProcessed = true;

	Movement->StopMovementImmediately();
	Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (ACharacterEnemy* Enemy = Cast<ACharacterEnemy>(OtherActor))
	{
		ApplyDamageAndBurn(Enemy, Hit);
	}

	Destroy();
}

void AFireballProjectile::ApplyDamageAndBurn(ACharacterEnemy * Enemy, const FHitResult & Hit)
{
	if (!IsValid(Enemy) || !DamageEffectClass || !BurnEffectClass) return;

	UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner());
	UAbilitySystemComponent* TargetASC = Enemy->GetAbilitySystemComponent();

	if (!IsValid(SourceASC) || !IsValid(TargetASC)) return;

	const FGameplayTag BurnTag = FGameplayTag::RequestGameplayTag(FName(TEXT("State.Burn")));
	const FGameplayTag DamageTag = FGameplayTag::RequestGameplayTag(FName(TEXT("Data.Damage")));

	const bool bAlreadyBurning = TargetASC->HasMatchingGameplayTag(BurnTag);

	const float ImmediateDamage = bAlreadyBurning ? 20.0f : 10.0f;

	FGameplayEffectContextHandle Context = SourceASC->MakeEffectContext();

	Context.AddHitResult(Hit, true);

	FGameplayEffectSpecHandle DamageSpec = SourceASC->MakeOutgoingSpec(DamageEffectClass, 1.0f, Context);

	if (DamageSpec.IsValid())
	{
		DamageSpec.Data->SetSetByCallerMagnitude(DamageTag, -ImmediateDamage);

		SourceASC->ApplyGameplayEffectSpecToTarget(*DamageSpec.Data.Get(), TargetASC);
	}

	FGameplayEffectSpecHandle BurnSpec = SourceASC->MakeOutgoingSpec( BurnEffectClass, 1.0f, Context);

	if (BurnSpec.IsValid())
	{
		SourceASC->ApplyGameplayEffectSpecToTarget(*BurnSpec.Data.Get(), TargetASC);
	}
}

