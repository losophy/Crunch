// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "CGameplayAbilityTypes.generated.h"

UENUM(BlueprintType)
enum class ECAbilityInputID : uint8
{
	None					UMETA(DisplayName = "None"),
	BasicAttack				UMETA(DisplayName = "Basic Attack"),
	AbilityOne				UMETA(DisplayName = "Ability One"),
	AbilityTwo				UMETA(DisplayName = "Ability Two"),
	AbilityThree			UMETA(DisplayName = "Ability Three"),
	AbilityFour				UMETA(DisplayName = "Ability Four"),
	AbilityFive				UMETA(DisplayName = "Ability Five"),
	AbilitySix				UMETA(DisplayName = "Ability Six"),
	Confirm					UMETA(DisplayName = "Confirm"),
	Cancel					UMETA(DisplayName = "Cancel"),
};

USTRUCT(BlueprintType)
struct FGenericDamgeEffectDef
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> DamageEffect;

	UPROPERTY(EditAnywhere)
	FVector PushVelocity;
};

USTRUCT(BlueprintType)
struct FHeroBaseStats : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> Class;

	UPROPERTY(EditAnywhere)
	float Strength;

	UPROPERTY(EditAnywhere)
	float Intelligence;

	UPROPERTY(EditAnywhere)
	float StrengthGrowthRate;

	UPROPERTY(EditAnywhere)
	float IntelligenceGrowthRate;

	UPROPERTY(EditAnywhere)
	float BaseMaxHealth;

	UPROPERTY(EditAnywhere)
	float BaseMaxMana;

	UPROPERTY(EditAnywhere)
	float BaseAttackDamage;

	UPROPERTY(EditAnywhere)
	float BaseArmor;

	UPROPERTY(EditAnywhere)
	float BaseMoveSpeed;
};
