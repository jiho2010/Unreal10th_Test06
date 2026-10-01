// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterPlayer.h"
#include "Camera/CameraComponent.h"
#include "Framework/GameHUD.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GAS/PlayerAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"

ACharacterPlayer::ACharacterPlayer()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	PlayerAttributeSet = CreateDefaultSubobject<UPlayerAttributeSet>(TEXT("PlayerAttributeSet"));
}

void ACharacterPlayer::ActivateFireball()
{
	if (!IsValid(AbilitySystemComponent) || !FireballAbilityClass)
	{
		return;
	}

	AbilitySystemComponent->TryActivateAbilityByClass(FireballAbilityClass);
}

UPlayerAttributeSet* ACharacterPlayer::GetPlayerAttribute() const
{
	return PlayerAttributeSet.Get();
}

void ACharacterPlayer::BeginPlay()
{
	Super::BeginPlay();
}

void ACharacterPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	GiveFireballAbility();

	if (APlayerController* PC = Cast<APlayerController>(NewController))
	{
		if (AGameHUD* GameHUD = Cast<AGameHUD>(PC->GetHUD()))
		{
			GameHUD->InitHUD(this);
		}
	}
}

void ACharacterPlayer::GiveFireballAbility()
{
	if (!HasAuthority() || !IsValid(AbilitySystemComponent) || !FireballAbilityClass)
	{
		return;
	}

	if (AbilitySystemComponent->FindAbilitySpecFromClass(FireballAbilityClass))
	{
		return;
	}

	FGameplayAbilitySpec Spec(FireballAbilityClass, 1, INDEX_NONE, this);

	AbilitySystemComponent->GiveAbility(Spec);
}
