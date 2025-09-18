// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA_Lazer.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_WaitCancel.h"
#include "Abilities/Tasks/AbilityTask_WaitTargetData.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GAS/CAttributeSet.h"
#include "GAS/TargetActor_Line.h"

void UGA_Lazer::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!K2_CommitAbility() || !LazerMontage)
	{
		K2_EndAbility();
		return;
	}

	if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		UAbilityTask_PlayMontageAndWait* PlayerLazerMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, LazerMontage);
		PlayerLazerMontageTask->OnBlendOut.AddDynamic(this, &UGA_Lazer::K2_EndAbility);
		PlayerLazerMontageTask->OnCancelled.AddDynamic(this, &UGA_Lazer::K2_EndAbility);
		PlayerLazerMontageTask->OnInterrupted.AddDynamic(this, &UGA_Lazer::K2_EndAbility);
		PlayerLazerMontageTask->OnCompleted.AddDynamic(this, &UGA_Lazer::K2_EndAbility);
		PlayerLazerMontageTask->ReadyForActivation();

		UAbilityTask_WaitGameplayEvent* WaitShootEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, GetShootTag());
		WaitShootEvent->EventReceived.AddDynamic(this, &UGA_Lazer::ShootLazer);
		WaitShootEvent->ReadyForActivation();

		UAbilityTask_WaitCancel* WaitCanel = UAbilityTask_WaitCancel::WaitCancel(this);
		WaitCanel->OnCancel.AddDynamic(this, &UGA_Lazer::K2_EndAbility);
		WaitCanel->ReadyForActivation();
	}
}

void UGA_Lazer::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	UAbilitySystemComponent* OwnerAbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	if (OwnerAbilitySystemComponent && OnGoingConsumtionEffectHandle.IsValid())
	{
		OwnerAbilitySystemComponent->RemoveActiveGameplayEffect(OnGoingConsumtionEffectHandle);
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

FGameplayTag UGA_Lazer::GetShootTag()
{
	return FGameplayTag::RequestGameplayTag("ability.lazer.shoot");
}

void UGA_Lazer::ShootLazer(FGameplayEventData Payload)
{
	if (K2_HasAuthority())
	{
		OnGoingConsumtionEffectHandle = BP_ApplyGameplayEffectToOwner(OnGoingConsumtionEffect, GetAbilityLevel(CurrentSpecHandle, CurrentActorInfo));
		UAbilitySystemComponent* OwnerAbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
		if (OwnerAbilitySystemComponent)
		{
			OwnerAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UCAttributeSet::GetManaAttribute()).AddUObject(this, &UGA_Lazer::ManaUpdated);
		}
	}

	UAbilityTask_WaitTargetData* WaitDamageTargetTask = UAbilityTask_WaitTargetData::WaitTargetData(this, NAME_None, EGameplayTargetingConfirmation::CustomMulti, LazerTargetActorClass);
	WaitDamageTargetTask->ValidData.AddDynamic(this, &UGA_Lazer::TargetReceived);
	WaitDamageTargetTask->ReadyForActivation();

	AGameplayAbilityTargetActor* TargetActor;
	WaitDamageTargetTask->BeginSpawningActor(this, LazerTargetActorClass, TargetActor);
	WaitDamageTargetTask->FinishSpawningActor(this, TargetActor);

	ATargetActor_Line* LineTargetActor = Cast<ATargetActor_Line>(TargetActor);
	if (LineTargetActor)
	{
		LineTargetActor->ConfigureTargetSetting(TargetRange, DetectionCylinderRadius, TargetingInterval, GetOwnerTeamId(), ShouldDrawDebug());
		LineTargetActor->AttachToComponent(GetOwningComponentFromActorInfo(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TargetActorAttachSocketName);
	}
}

void UGA_Lazer::ManaUpdated(const FOnAttributeChangeData& ChangeData)
{
	UAbilitySystemComponent* OwnerAbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	if (OwnerAbilitySystemComponent && !OwnerAbilitySystemComponent->CanApplyAttributeModifiers(OnGoingConsumtionEffect.GetDefaultObject(), GetAbilityLevel(CurrentSpecHandle, CurrentActorInfo), MakeEffectContext(CurrentSpecHandle, CurrentActorInfo)))
	{
		K2_EndAbility();
	}
}

void UGA_Lazer::TargetReceived(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
}
