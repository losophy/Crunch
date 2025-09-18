// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/CGameplayAbility.h"
#include "GA_Lazer.generated.h"

/**
 * 
 */
UCLASS()
class UGA_Lazer : public UCGameplayAbility
{
	GENERATED_BODY()
public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	static FGameplayTag GetShootTag();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	TSubclassOf<UGameplayEffect> OnGoingConsumtionEffect;

	FActiveGameplayEffectHandle OnGoingConsumtionEffectHandle;

	UPROPERTY(EditDefaultsOnly, Category = "Anim")
	class UAnimMontage* LazerMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Targeting")
	FName TargetActorAttachSocketName = "Lazer";

	UPROPERTY(EditDefaultsOnly, Category = "Targeting")
	TSubclassOf<class ATargetActor_Line> LazerTargetActorClass;

	UFUNCTION()
	void ShootLazer(FGameplayEventData Payload);
	void ManaUpdated(const FOnAttributeChangeData& ChangeData);

	UFUNCTION()
	void TargetReceived(const FGameplayAbilityTargetDataHandle& TargetDataHandle);
};
