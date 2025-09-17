// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "TargetActor_Line.generated.h"

/**
 * 
 */
UCLASS()
class ATargetActor_Line : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()
public:
	ATargetActor_Line();
private:
	UPROPERTY(VisibleDefaultsOnly, Category = "Component")
	class USceneComponent* RootComp;

	UPROPERTY(VisibleDefaultsOnly, Category = "Component")
	class UNiagaraComponent* LazerVFX;

	UPROPERTY(VisibleDefaultsOnly, Category = "Component")
	class USphereComponent* TargetEndDetectionSphere;

	const AActor* AvatarActor;
};
