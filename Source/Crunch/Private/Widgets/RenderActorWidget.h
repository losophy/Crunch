// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RenderActorWidget.generated.h"

class ARenderActor;
/**
 * 
 */
UCLASS(Abstract)
class URenderActorWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void BeginDestroy() override;

private:
	void ConfigureRenderActor();
	virtual void SpawnRenderActor() PURE_VIRTUAL(URenderActorWidget::SpawnRenderActor, );
	virtual ARenderActor* GetRenderActor() const PURE_VIRTUAL(URenderActorWidget::GetRenderActor, return nullptr; );

	void BeginRenderCapture();
	void UpdateRender();
	void StopRenderCapture();

	UPROPERTY(meta = (BindWidget))
	class UImage* DisplayImage;

	UPROPERTY(meta = (BindWidget))
	class USizeBox* RenderSizeBox;

	UPROPERTY(EditDefaultsOnly, Category = "Render Actor")
	FName DisplayImageRenderTargetParamName = "RenderTarget";

	UPROPERTY(EditDefaultsOnly, Category = "Render Actor")
	FVector2D RenderSize;

	UPROPERTY(EditDefaultsOnly, Category = "Render Actor")
	int FrameRate = 24;

	float RenderTickInterval;
	FTimerHandle RenderTimerHandle;

	UPROPERTY()
	UTextureRenderTarget2D* RenderTarget;
};
