// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/TA_Blackhole.h"
#include "Components/SphereComponent.h"
#include "Components/SceneComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Net/UnrealNetwork.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

ATA_Blackhole::ATA_Blackhole()
{
	RootComp = CreateDefaultSubobject<USceneComponent>("Root Comp");
	SetRootComponent(RootComp);

	DetectionSphereComponent = CreateDefaultSubobject<USphereComponent>("Detection Sphere Component");
	DetectionSphereComponent->SetupAttachment(GetRootComponent());
	DetectionSphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	DetectionSphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	DetectionSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ATA_Blackhole::ActorInBlackholeRange);
	DetectionSphereComponent->OnComponentEndOverlap.AddDynamic(this, &ATA_Blackhole::ActorLeftBlackholeRange);

	bReplicates = true;
	ShouldProduceTargetDataOnServer = true;
	PrimaryActorTick.bCanEverTick = true;

	VFXComponent = CreateDefaultSubobject<UParticleSystemComponent>("VFX Component");
	VFXComponent->SetupAttachment(GetRootComponent());
}

void ATA_Blackhole::ConfigureBlackhole(float InBlackholeRange, float InPullSpeed, float InBlackholeDuration, const FGenericTeamId& InTeamId)
{
	PullSpeed = InPullSpeed;
	DetectionSphereComponent->SetSphereRadius(InBlackholeRange);
	SetGenericTeamId(InTeamId);
	BlackholeDuration = InBlackholeDuration;
	BlackholeRange = InBlackholeRange;
}

void ATA_Blackhole::SetGenericTeamId(const FGenericTeamId& InTeamId)
{
	TeamId = InTeamId;
}

void ATA_Blackhole::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ATA_Blackhole, TeamId);
	DOREPLIFETIME_CONDITION_NOTIFY(ATA_Blackhole, BlackholeRange, COND_None, REPNOTIFY_Always);
}

void ATA_Blackhole::OnRep_BlackholeRange()
{
	DetectionSphereComponent->SetSphereRadius(BlackholeRange);
}

void ATA_Blackhole::ActorInBlackholeRange(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void ATA_Blackhole::ActorLeftBlackholeRange(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}
