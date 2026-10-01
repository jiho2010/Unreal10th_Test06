// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "CharacterEnemy.generated.h"

class UWidgetComponent;
class UEnemyAttributeSet;

/**
 * 
 */
UCLASS()
class UNREAL10TH_TEST06_API ACharacterEnemy : public ABaseCharacter
{
	GENERATED_BODY()

public:
	ACharacterEnemy();

	UFUNCTION(BlueprintPure, Category = "GAS")
	UEnemyAttributeSet* GetUEnemyAttribute() const;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void InitializeOverHeadWidget();

	virtual void UpdateOverheadWidgetRotation();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "OverHead")
	TObjectPtr<UWidgetComponent> OverHeadWidgetComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	TObjectPtr<UEnemyAttributeSet> EnemyAttributeSet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Overhead")
	bool bFaceCamera = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Overhead")
	bool bLockWidgetPitch = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Overhead")
	bool bLockWidgetRoll = true;
};
