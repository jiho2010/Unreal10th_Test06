// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OverHeadWidget.generated.h"

class UAbilitySystemComponent;
class UTextBlock;
class UProgressBar;
struct FOnAttributeChangeData;

/**
 * 
 */
UCLASS()
class UNREAL10TH_TEST06_API UOverHeadWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "GAS")
	void InitializeWithAbilitySystem(AActor* InActor);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> HealthText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> HealthProgressBar;

private:
	void OnAttributeChanged(const FOnAttributeChangeData& Data);
	void RefreshUI();
	void UnbindAttributeDelegates();

	TWeakObjectPtr<AActor> SourceActor;
	TWeakObjectPtr<UAbilitySystemComponent> ASC;
	
};
