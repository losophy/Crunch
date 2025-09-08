// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/StormCore.h"
#include "AIController.h"
#include "Components/SphereComponent.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AStormCore::AStormCore()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	InfluenceRange = CreateDefaultSubobject<USphereComponent>("Influence Range");
	InfluenceRange->SetupAttachment(GetRootComponent());

	InfluenceRange->OnComponentBeginOverlap.AddDynamic(this, &AStormCore::NewInfluenerInRange);
	InfluenceRange->OnComponentEndOverlap.AddDynamic(this, &AStormCore::InfluencerLeftRange);
}

// Called when the game starts or when spawned
void AStormCore::BeginPlay()
{
	Super::BeginPlay();
	
}

void AStormCore::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	OwnerAIC = Cast<AAIController>(NewController);
}

// Called every frame
void AStormCore::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AStormCore::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AStormCore::NewInfluenerInRange(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IGenericTeamAgentInterface* OtherTeamInterface = Cast<IGenericTeamAgentInterface>(OtherActor);
	if (OtherTeamInterface)
	{
		if (OtherTeamInterface->GetGenericTeamId().GetId() == 0)
		{
			TeamOneInfluncerCount++;
		}
		else if (OtherTeamInterface->GetGenericTeamId().GetId() == 1)
		{
			TeamTwoInfluncerCount++;
		}
		UpdateTeamWeight();
	}
}

void AStormCore::InfluencerLeftRange(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	IGenericTeamAgentInterface* OtherTeamInterface = Cast<IGenericTeamAgentInterface>(OtherActor);
	if (OtherTeamInterface)
	{
		if (OtherTeamInterface->GetGenericTeamId().GetId() == 0)
		{
			TeamOneInfluncerCount--;
			if (TeamOneInfluncerCount < 0)
			{
				TeamOneInfluncerCount = 0;
			}
		}
		else if (OtherTeamInterface->GetGenericTeamId().GetId() == 1)
		{
			TeamTwoInfluncerCount--;
			if (TeamTwoInfluncerCount < 0)
			{
				TeamTwoInfluncerCount = 0;
			}
		}
		UpdateTeamWeight();
	}
}

void AStormCore::UpdateTeamWeight()
{
	if (TeamOneInfluncerCount == TeamTwoInfluncerCount)
	{
		TeamWeight = 0.f;
	}
	else
	{
		float TeamOffset = TeamOneInfluncerCount - TeamTwoInfluncerCount;
		float TeamTotal = TeamOneInfluncerCount + TeamTwoInfluncerCount;

		TeamWeight = TeamOffset / TeamTotal;
	}

	UE_LOG(LogTemp, Warning, TEXT("Team One Count: %d, Team Two Count: %d, Weight: %f"), TeamOneInfluncerCount, TeamTwoInfluncerCount, TeamWeight);
	UpdateGoal();
}

void AStormCore::UpdateGoal()
{
	if (!HasAuthority())
		return;

	if (!OwnerAIC)
		return;

	if (!GetCharacterMovement())
		return;

	if (TeamWeight > 0)
	{
		OwnerAIC->MoveToActor(TeamOneGoal);
	}
	else
	{
		OwnerAIC->MoveToActor(TeamTwoGoal);
	}

	float Speed = MaxMoveSpeed * FMath::Abs(TeamWeight);

	GetCharacterMovement()->MaxWalkSpeed = Speed;
}
