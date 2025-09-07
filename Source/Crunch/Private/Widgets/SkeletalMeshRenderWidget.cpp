// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/SkeletalMeshRenderWidget.h"
#include "GameFramework/Character.h"
#include "Widgets/SkeletalMeshRenderActor.h"

void USkeletalMeshRenderWidget::NativeConstruct()
{
	Super::NativeConstruct();
	ACharacter* PlayerCharacter = GetOwningPlayerPawn<ACharacter>();
	if (PlayerCharacter && SkeletalMeshRenderActor)
	{
		SkeletalMeshRenderActor->ConfigureSkeletalMesh(PlayerCharacter->GetMesh()->GetSkeletalMeshAsset(), PlayerCharacter->GetMesh()->GetAnimClass());
	}
}

void USkeletalMeshRenderWidget::SpawnRenderActor()
{
	if (!SKeletalMeshRenderActorClass)
		return;

	UWorld* World = GetWorld();
	if (!World)
		return;

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	SkeletalMeshRenderActor = World->SpawnActor<ASkeletalMeshRenderActor>(SKeletalMeshRenderActorClass, SpawnParams);
}

ARenderActor* USkeletalMeshRenderWidget::GetRenderActor() const
{
	return SkeletalMeshRenderActor;
}
