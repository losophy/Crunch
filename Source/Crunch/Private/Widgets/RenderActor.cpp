// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/RenderActor.h"
#include "Components/SceneComponent.h"
#include "Components/SceneCaptureComponent2D.h"

// Sets default values
ARenderActor::ARenderActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComp = CreateDefaultSubobject<USceneComponent>("Root Comp");
	SetRootComponent(RootComp);

	CaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>("Capture Component");
	CaptureComponent->SetupAttachment(RootComp);

	CaptureComponent->bCaptureEveryFrame = false;
	CaptureComponent->FOVAngle = 30.f;
}

void ARenderActor::SetRenderTarget(UTextureRenderTarget2D* RenderTarget)
{
	CaptureComponent->TextureTarget = RenderTarget;
}

void ARenderActor::UpdateRender()
{
	if (CaptureComponent)
	{
		CaptureComponent->CaptureScene();
	}
}

// Called when the game starts or when spawned
void ARenderActor::BeginPlay()
{
	Super::BeginPlay();
	CaptureComponent->ShowOnlyActorComponents(this);
}
