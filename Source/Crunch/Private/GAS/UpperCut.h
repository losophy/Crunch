// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/CGameplayAbility.h"
#include "UpperCut.generated.h"

/**
 * 
 */
UCLASS()
class UUpperCut : public UCGameplayAbility
{
	GENERATED_BODY()
public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	UUpperCut();
private:
	UPROPERTY(EditDefaultsOnly, Category = "Launch")
	TSubclassOf<UGameplayEffect> LaunchDamageEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Launch")
	float UpperCutLaunchSpeed = 1000.f;

	UPROPERTY(EditDefaultsOnly, Category = "Targeting")
	float TargetSweepSphereRadius = 80.f;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* UpperCutMontage;

	static FGameplayTag GetUpperCutLaunchTag();

	UFUNCTION()
	void StartLaunching(FGameplayEventData EventData);

	UFUNCTION()
	void HandleComboChangeEvent(FGameplayEventData EventData);

	UFUNCTION()
	void HandleComboCommitEvent(FGameplayEventData EventData);

	FName NextComboName;
};
