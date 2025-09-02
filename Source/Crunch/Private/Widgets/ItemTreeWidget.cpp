// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/ItemTreeWidget.h"
#include "Widgets/SplineWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"

void UItemTreeWidget::ClearTree()
{
	RootPanel->ClearChildren();
}

UUserWidget* UItemTreeWidget::CreateWidgetForNode(const ITreeNodeInterface* Node, UCanvasPanelSlot*& OutCanvasSlot)
{
	if (!Node)
		return nullptr;

	UUserWidget* NodeWidget = Node->GetWidget();
	OutCanvasSlot = RootPanel->AddChildToCanvas(NodeWidget);
	if (OutCanvasSlot)
	{
		OutCanvasSlot->SetSize(NodeSize);
		OutCanvasSlot->SetAnchors(FAnchors(0.5f));
		OutCanvasSlot->SetAlignment(FVector2D(0.5f));
		OutCanvasSlot->SetZOrder(1);
	}

	return NodeWidget;
}

void UItemTreeWidget::CreateConnection(const UUserWidget* From, UUserWidget* To)
{
	if (!From || !To)
		return;

	USplineWidget* Connection = CreateWidget<USplineWidget>(GetOwningPlayer());
	UCanvasPanelSlot* ConnectionPanelSlot = RootPanel->AddChildToCanvas(Connection);

	if (ConnectionPanelSlot)
	{
		ConnectionPanelSlot->SetAnchors(FAnchors{ 0.f });
		ConnectionPanelSlot->SetAlignment(FVector2D{ 0.f });
		ConnectionPanelSlot->SetPosition(FVector2D::Zero());
		ConnectionPanelSlot->SetZOrder(0);
	}

	Connection->SetupSpline(From, To, SourePortLocalPos, DestinationPortLocalPos, SourcePortDirection, DestinationPortDirection);
	Connection->SetSplineStyle(ConnectionColor, ConnectionThickness);
}
