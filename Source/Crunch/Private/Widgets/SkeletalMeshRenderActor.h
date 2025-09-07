// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/RenderActor.h"
#include "SkeletalMeshRenderActor.generated.h"

/**
 * 
 */
UCLASS()
class ASkeletalMeshRenderActor : public ARenderActor
{
	GENERATED_BODY()
public:
	ASkeletalMeshRenderActor();
	void ConfigureSkeletalMesh(USkeletalMesh* MeshAsset, TSubclassOf<UAnimInstance> AnimBlueprint);
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Skeletal Mesh Renderer")
	class USkeletalMeshComponent* MeshComp;
};
