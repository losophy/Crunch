// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/AbilityGauge.h"
#include "Abilities/GameplayAbility.h"
#include "GAS/CAbilitySystemStatics.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UAbilityGauge::NativeConstruct()
{
	CooldownCounterText->SetVisibility(ESlateVisibility::Hidden);
}

void UAbilityGauge::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
	AbilityCD0 = Cast<UGameplayAbility>(ListItemObject);

	float CooldownDuration = UCAbilitySystemStatics::GetStaticCooldownDurationForAbility(AbilityCD0);
	float Cost = UCAbilitySystemStatics::GetStaticCostForAbility(AbilityCD0);

	CooldownDurationText->SetText(FText::AsNumber(CooldownDuration));
	CostText->SetText(FText::AsNumber(Cost));
}

void UAbilityGauge::ConfigureWithWidgetData(const FAbilityWidgetData* WidgetData)
{
	if (Icon && WidgetData)
	{
		Icon->GetDynamicMaterial()->SetTextureParameterValue(IconMaterialParamName, WidgetData->Icon.LoadSynchronous());
	}
}
