// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/CAbilitySystemStatics.h"
#include "Abilities/GameplayAbility.h"

FGameplayTag UCAbilitySystemStatics::GetBasicAttackAbilityTag()
{
	return FGameplayTag::RequestGameplayTag("ability.basicattack");
}

FGameplayTag UCAbilitySystemStatics::GetBasicAttackInputPressedTag()
{
	return FGameplayTag::RequestGameplayTag("ability.basicattack.pressed");
}

FGameplayTag UCAbilitySystemStatics::GetDeadStatTag()
{
	return FGameplayTag::RequestGameplayTag("stats.dead");
}

FGameplayTag UCAbilitySystemStatics::GetStunStatTag()
{
	return FGameplayTag::RequestGameplayTag("stats.stun");
}

FGameplayTag UCAbilitySystemStatics::GetAimStatTag()
{
	return FGameplayTag::RequestGameplayTag("stats.aim");
}

FGameplayTag UCAbilitySystemStatics::GetCameraShakeGameplayCueTag()
{
	return FGameplayTag::RequestGameplayTag("GameplayCue.cameraShake");
}

float UCAbilitySystemStatics::GetStaticCooldownDurationForAbility(const UGameplayAbility* Ability)
{
	if (!Ability)
		return 0.f;

	const UGameplayEffect* CooldownEffect = Ability->GetCooldownGameplayEffect();
	if (!CooldownEffect)
		return 0.f;

	float CooldownDuration = 0.f;

	CooldownEffect->DurationMagnitude.GetStaticMagnitudeIfPossible(1, CooldownDuration);
	return CooldownDuration;
}

float UCAbilitySystemStatics::GetStaticCostForAbility(const UGameplayAbility* Ability)
{
	if (!Ability)
		return 0.f;

	const UGameplayEffect* CostEffect = Ability->GetCostGameplayEffect();
	if (!CostEffect || CostEffect->Modifiers.Num() == 0)
		return 0.f;

	float Cost = 0.f;
	CostEffect->Modifiers[0].ModifierMagnitude.GetStaticMagnitudeIfPossible(1, Cost);
	return FMath::Abs(Cost);
}
