// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "StormCore.generated.h"

UCLASS()
class AStormCore : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AStormCore();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
private:
	UPROPERTY(EditDefaultsOnly, Category = "Move")
	float InfluenceRadius = 1000.f;

	UPROPERTY(EditDefaultsOnly, Category = "Move")
	float MaxMoveSpeed = 500.f;

	UPROPERTY(VisibleDefaultsOnly, Category = "Detection")
	class USphereComponent* InfluenceRange;

	UPROPERTY(VisibleDefaultsOnly, Category = "Detection")
	class UDecalComponent* GroundDecalComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = "Detection")
	class UCameraComponent* ViewCam;

	UFUNCTION()
	void NewInfluenerInRange(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void InfluencerLeftRange(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void UpdateTeamWeight();
	void UpdateGoal();

	UPROPERTY(EditAnywhere, Category = "Team")
	AActor* TeamOneGoal;

	UPROPERTY(EditAnywhere, Category = "Team")
	AActor* TeamTwoGoal;

	int TeamOneInfluncerCount = 0;
	int TeamTwoInfluncerCount = 0;

	float TeamWeight = 0.f;

	UPROPERTY()
	class AAIController* OwnerAIC;
};
