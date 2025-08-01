// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MinionBarrack.h"
#include "AI/Minion.h"
#include "GameFramework/PLayerStart.h"

// Sets default values
AMinionBarrack::AMinionBarrack()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMinionBarrack::BeginPlay()
{
	Super::BeginPlay();
	SpawnNewMinions(5);
}

// Called every frame
void AMinionBarrack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

const APlayerStart* AMinionBarrack::GetNextSpawnSpot()
{
	if (SpawnSpots.Num() == 0)
	{
		return nullptr;
	}

	++NextSpawnSpotIndex;

	if (NextSpawnSpotIndex >= SpawnSpots.Num())
	{
		NextSpawnSpotIndex = 0;
	}

	return SpawnSpots[NextSpawnSpotIndex];
}

void AMinionBarrack::SpawnNewMinions(int Amt)
{
	for (int i = 0; i < Amt; i++)
	{
		FTransform SpawnTransfrom = GetActorTransform();
		if (const APlayerStart* NextSpawnSpot = GetNextSpawnSpot())
		{
			SpawnTransfrom = NextSpawnSpot->GetActorTransform();
		}	

		AMinion* NewMinion = GetWorld()->SpawnActorDeferred<AMinion>(MinionClass, SpawnTransfrom, this, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
		NewMinion->SetGenericTeamId(BarrackTeamId);
		NewMinion->FinishSpawning(SpawnTransfrom);
		MinionPool.Add(NewMinion);
	}
}

