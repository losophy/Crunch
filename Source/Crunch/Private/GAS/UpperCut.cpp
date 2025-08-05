// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/UpperCut.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "GAS/GA_Combo.h"
#include "GameplayTagsManager.h"

void UUpperCut::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!K2_CommitAbility())
	{
		K2_EndAbility();
		return;
	}
		
	if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		UAbilityTask_PlayMontageAndWait* PlayUpperCutMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, UpperCutMontage);
		PlayUpperCutMontageTask->OnBlendOut.AddDynamic(this, &UUpperCut::K2_EndAbility);
		PlayUpperCutMontageTask->OnCancelled.AddDynamic(this, &UUpperCut::K2_EndAbility);
		PlayUpperCutMontageTask->OnInterrupted.AddDynamic(this, &UUpperCut::K2_EndAbility);
		PlayUpperCutMontageTask->OnCompleted.AddDynamic(this, &UUpperCut::K2_EndAbility);
		PlayUpperCutMontageTask->ReadyForActivation();

		UAbilityTask_WaitGameplayEvent* WaitLaunchEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, GetUpperCutLaunchTag());
		WaitLaunchEventTask->EventReceived.AddDynamic(this, &UUpperCut::StartLaunching);
		WaitLaunchEventTask->ReadyForActivation();
	}
	NextComboName = NAME_None;
}

FGameplayTag UUpperCut::GetUpperCutLaunchTag()
{
	return FGameplayTag::RequestGameplayTag("ability.uppercut.launch");
}

void UUpperCut::StartLaunching(FGameplayEventData EventData)
{
	if (K2_HasAuthority())
	{
		TArray<FHitResult> TargetHitResults = GetHitResultFromSweepLocationTargetData(EventData.TargetData, TargetSweepSphereRadius, ETeamAttitude::Hostile, ShouldDrawDebug());
		PushTarget(GetAvatarActorFromActorInfo(), FVector::UpVector * UpperCutLaunchSpeed);
		for (FHitResult& HitResult : TargetHitResults)
		{
			PushTarget(HitResult.GetActor(), FVector::UpVector * UpperCutLaunchSpeed);
			ApplyGameplayEffectToHitResultActor(HitResult, LaunchDamageEffect, GetAbilityLevel(CurrentSpecHandle, CurrentActorInfo));
		}	
	}

	UAbilityTask_WaitGameplayEvent* WaitComboChangeEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, UGA_Combo::GetComboChangedEventTag(), nullptr, false, false);
	WaitComboChangeEvent->EventReceived.AddDynamic(this, &UUpperCut::HandleComboChangeEvent);
	WaitComboChangeEvent->ReadyForActivation();
}

void UUpperCut::HandleComboChangeEvent(FGameplayEventData EventData)
{
	FGameplayTag EventTag = EventData.EventTag;
	if (EventTag == UGA_Combo::GetComboChangedEventEndTag())
	{
		NextComboName = NAME_None;
		UE_LOG(LogTemp, Warning, TEXT("Next Combo is cleared"));
		return;
	}

	TArray<FName> TagNames;
	UGameplayTagsManager::Get().SplitGameplayTagFName(EventTag, TagNames);

	NextComboName = TagNames.Last();
	UE_LOG(LogTemp, Warning, TEXT("Next Combo is: %s"), *NextComboName.ToString());
}
