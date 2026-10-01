// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidget.generated.h"

class UPlayerStatWidget;

/**
 * 
 */
UCLASS()
class UNREAL10TH_TEST06_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "GAS")
	void InitializeWithAbilitySystem(AActor* InActor);

protected:
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	TObjectPtr<UPlayerStatWidget> PlayerStatWidget;
};
