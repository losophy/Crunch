// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/CharacterDisplay.h"
#include "Camera/CameraComponent.h"
#include "Character/PA_CharacterDefination.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
ACharacterDisplay::ACharacterDisplay()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("Root Comp"));

	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh Component");
	MeshComponent->SetupAttachment(GetRootComponent());

	ViewCameraComponent = CreateDefaultSubobject<UCameraComponent>("View Camera Component");
	ViewCameraComponent->SetupAttachment(GetRootComponent());
}

void ACharacterDisplay::ConfigureWithCharacterDefination(const UPA_CharacterDefination* CharacterDefination)
{
	if (!CharacterDefination)
		return;

	MeshComponent->SetSkeletalMesh(CharacterDefination->LoadDisplayMesh());
	MeshComponent->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	MeshComponent->SetAnimClass(CharacterDefination->LoadDisplayAnimationBP());
}

