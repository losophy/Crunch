// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/TargetActor_GroundPick.h"
#include "Crunch/Crunch.h"

ATargetActor_GroundPick::ATargetActor_GroundPick()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATargetActor_GroundPick::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (PrimaryPC && PrimaryPC->IsLocalPlayerController())
	{
		SetActorLocation(GetTargetPoint());
	}
}

FVector ATargetActor_GroundPick::GetTargetPoint() const
{
	if (!PrimaryPC || !PrimaryPC->IsLocalPlayerController())
		return GetActorLocation();

	FHitResult TraceResult;

	FVector ViewLoc;
	FRotator ViewRot;

	PrimaryPC->GetPlayerViewPoint(ViewLoc, ViewRot);

	FVector TraceEnd = ViewLoc + ViewRot.Vector() * TargetTraceRange;

	GetWorld()->LineTraceSingleByChannel(TraceResult, ViewLoc, TraceEnd, ECC_Target);

	if (!TraceResult.bBlockingHit)
	{
		GetWorld()->LineTraceSingleByChannel(TraceResult, TraceEnd, TraceEnd + FVector::DownVector * TNumericLimits<float>::Max(), ECC_Target);
	}	

	if (!TraceResult.bBlockingHit)
	{
		return GetActorLocation();
	}	

	return TraceResult.ImpactPoint;
}
