// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/TargetActor_Line.h"
#include "Crunch/Crunch.h"
#include "Components/SphereComponent.h"
#include "Components/SceneComponent.h"
#include "NiagaraComponent.h"
#include "Net/UnrealNetwork.h"
#include "Abilities/GameplayAbility.h"
#include "Kismet/KismetMathLibrary.h"

ATargetActor_Line::ATargetActor_Line()
{
	RootComp = CreateDefaultSubobject<USceneComponent>("Root Comp");
	SetRootComponent(RootComp);

	TargetEndDetectionSphere = CreateDefaultSubobject<USphereComponent>("Target End Detection Sphere");
	TargetEndDetectionSphere->SetupAttachment(GetRootComponent());
	TargetEndDetectionSphere->SetCollisionResponseToChannel(ECC_SpringArm, ECR_Ignore);

	LazerVFX = CreateDefaultSubobject<UNiagaraComponent>("Lazer VFX");
	LazerVFX->SetupAttachment(GetRootComponent());

	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	ShouldProduceTargetDataOnServer = true;

	AvatarActor = nullptr;
}

void ATargetActor_Line::ConfigureTargetSetting(float NewTargetRange, float NewDetectionCylinderRadius, float NewTargetingInterval, FGenericTeamId OwnerTeamId, bool bShouldDrawDebug)
{
	TargetRange = NewTargetRange;
	DetectionCylinderRadius = NewDetectionCylinderRadius;
	TargetingInterval = NewTargetingInterval;
	SetGenericTeamId(OwnerTeamId);
	bDrawDebug = bShouldDrawDebug;
}

void ATargetActor_Line::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	TeamId = NewTeamID;
}

void ATargetActor_Line::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ATargetActor_Line, TeamId);
	DOREPLIFETIME(ATargetActor_Line, TargetRange);
	DOREPLIFETIME(ATargetActor_Line, DetectionCylinderRadius);
	DOREPLIFETIME(ATargetActor_Line, AvatarActor);
}
