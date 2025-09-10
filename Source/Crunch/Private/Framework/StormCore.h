// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "StormCore.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnGoalReachedDelegate, AActor* /*ViewTarget*/, int /*WiningTeam*/);
DECLARE_MULTICAST_DELEGATE_TwoParams(FonTeamInfluncerCountUpdatedDelegate, int /*TeamOneInfluencerCount*/, int /*TeamTwoInfluencerCount*/);

UCLASS()
class AStormCore : public ACharacter
{
	GENERATED_BODY()

public:
	FOnGoalReachedDelegate OnGoalReachedDelegate;
	FonTeamInfluncerCountUpdatedDelegate OnTeamInfluenceCountUpdated;

	// Sets default values for this character's properties
	AStormCore();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	float GetProgress() const;

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
	UAnimMontage* ExpandMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Move")
	UAnimMontage* CaptureMontage;

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

	UPROPERTY(EditAnywhere, Category = "Team")
	AActor* TeamOneCore;

	UPROPERTY(EditAnywhere, Category = "Team")
	AActor* TeamTwoCore;

	UPROPERTY(ReplicatedUsing = OnRep_CoreToCapture)
	AActor* CoreToCapture;

	float CoreCaptureSpeed = 0.f;
	float TravelLength = 0.f;

	UFUNCTION()
	void OnRep_CoreToCapture();

	void GoalReached(int WiningTeam);

	void CaptureCore();
	void ExpandFinished();

	int TeamOneInfluncerCount = 0;
	int TeamTwoInfluncerCount = 0;

	float TeamWeight = 0.f;

	UPROPERTY()
	class AAIController* OwnerAIC;
};
