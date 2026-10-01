// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "CharacterPlayer.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UPlayerAttributeSet;
class UGameplayAbility;

/**
 * 
 */
UCLASS()
class UNREAL10TH_TEST06_API ACharacterPlayer : public ABaseCharacter
{
	GENERATED_BODY()
	
public:
	ACharacterPlayer();

	void ActivateFireball();

	UFUNCTION(BlueprintPure, Category = "GAS")
	UPlayerAttributeSet* GetPlayerAttribute() const;

protected:
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;

	void GiveFireballAbility();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	TObjectPtr<UPlayerAttributeSet> PlayerAttributeSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS")
	TSubclassOf<UGameplayAbility> FireballAbilityClass;

};
