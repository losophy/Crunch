// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/CGameplayAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GAS/GAP_Launched.h"
#include "GAS/CAbilitySystemStatics.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"

UCGameplayAbility::UCGameplayAbility()
{
    ActivationBlockedTags.AddTag(UCAbilitySystemStatics::GetStunStatTag());
}

bool UCGameplayAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
    FGameplayAbilitySpec* AbilitySpec = ActorInfo->AbilitySystemComponent->FindAbilitySpecFromHandle(Handle);
    if (AbilitySpec && AbilitySpec->Level <= 0)
    {
        return false;
    }

    return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

AActor* UCGameplayAbility::GetAimTarget(float AimDistance, ETeamAttitude::Type TeamAttitude) const
{
    AActor* OwnerAvatarActor = GetAvatarActorFromActorInfo();
    if (OwnerAvatarActor)
    {
        FVector Location;
        FRotator Rotation;
        OwnerAvatarActor->GetActorEyesViewPoint(Location, Rotation);

        FVector AimEnd = Location + Rotation.Vector() * AimDistance;

        FCollisionQueryParams CollisionQueryParams;
        CollisionQueryParams.AddIgnoredActor(OwnerAvatarActor);

        FCollisionObjectQueryParams CollisionObjectQueryParams;
        CollisionObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);

        if (ShouldDrawDebug())
        {
            DrawDebugLine(GetWorld(), Location, AimEnd, FColor::Red, false, 2.f, 0U, 3.f);
        }

        TArray<FHitResult> HitResults;
        if (GetWorld()->LineTraceMultiByObjectType(HitResults, Location, AimEnd, CollisionObjectQueryParams, CollisionQueryParams))
        {
            for (FHitResult& HitResult : HitResults)
            {
                if (IsActorTeamAttitudeIs(HitResult.GetActor(), TeamAttitude))
                {
                    return HitResult.GetActor();
                }
            }
        }
    }

    return nullptr;
}

UAnimInstance* UCGameplayAbility::GetOwnerAnimInstance() const
{
    USkeletalMeshComponent* OwnerSkeletalMeshComp = GetOwningComponentFromActorInfo();
    if (OwnerSkeletalMeshComp)
    {
        return OwnerSkeletalMeshComp->GetAnimInstance();
    }
    return nullptr;
}

TArray<FHitResult> UCGameplayAbility::GetHitResultFromSweepLocationTargetData(const FGameplayAbilityTargetDataHandle& TargetDataHandle, float SphereSweepRadius, ETeamAttitude::Type TargetTeam, bool bDrawDebug, bool bIgnoreSelf) const
{
    TArray<FHitResult> OutResults;
    TSet<AActor*> HitActors;

    IGenericTeamAgentInterface* OwnerTeamInterface = Cast<IGenericTeamAgentInterface>(GetAvatarActorFromActorInfo());
     

    for (const TSharedPtr<FGameplayAbilityTargetData> TargetData : TargetDataHandle.Data)
    {
        FVector StartLoc = TargetData->GetOrigin().GetTranslation();
        FVector EndLoc = TargetData->GetEndPoint();

        TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
        ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

        TArray<AActor*> ActorsToIgnore;
        if (bIgnoreSelf)
        {
            ActorsToIgnore.Add(GetAvatarActorFromActorInfo());
        }

        EDrawDebugTrace::Type DrawDebugTrace = bDrawDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;

        TArray<FHitResult> Results;
        UKismetSystemLibrary::SphereTraceMultiForObjects(this, StartLoc, EndLoc, SphereSweepRadius, ObjectTypes, false, ActorsToIgnore, DrawDebugTrace, Results, false);

        for (const FHitResult& Result : Results)
        {
            if (HitActors.Contains(Result.GetActor()))
            {
                continue;
            }

            if (OwnerTeamInterface)
            {
                ETeamAttitude::Type OtherActorTeamAttitude = OwnerTeamInterface->GetTeamAttitudeTowards(*Result.GetActor());
                if (OtherActorTeamAttitude != TargetTeam)
                {
                    continue;
                } 
            } 

            HitActors.Add(Result.GetActor());
            OutResults.Add(Result);
        }
    }

    return OutResults;
}

void UCGameplayAbility::PushSelf(const FVector& PushVel)
{
    ACharacter* OwningAvatarCharacter = GetOwningAvatarCharacter();
    if (OwningAvatarCharacter)
    {
        OwningAvatarCharacter->LaunchCharacter(PushVel, true, true);
    }
}

void UCGameplayAbility::PushTarget(AActor* Target, const FVector& PushVel)
{
    if (!Target)
        return;

    FGameplayEventData EventData;
    FGameplayAbilityTargetData_SingleTargetHit* HitData = new FGameplayAbilityTargetData_SingleTargetHit;
    FHitResult HitResult;
    HitResult.ImpactNormal = PushVel;
    HitData->HitResult = HitResult;
    EventData.TargetData.Add(HitData);

    UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Target, UGAP_Launched::GetLauchedAbilityActiationTag(), EventData);
}

void UCGameplayAbility::PushTargets(const TArray<AActor*>& Targets, const FVector& PushVel)
{
    for (AActor* Target : Targets)
    {
        PushTarget(Target, PushVel);
    } 
}

void UCGameplayAbility::PushTargets(const FGameplayAbilityTargetDataHandle& TargetDataHandle, const FVector& PushVel)
{
    TArray<AActor*> Targets = UAbilitySystemBlueprintLibrary::GetAllActorsFromTargetData(TargetDataHandle);
    PushTargets(Targets, PushVel);
}

void UCGameplayAbility::PlayMontageLocally(UAnimMontage* MontageToPlay)
{
    UAnimInstance* OwnerAnimInst = GetOwnerAnimInstance();
    if (OwnerAnimInst && !OwnerAnimInst->Montage_IsPlaying(MontageToPlay))
    {
        OwnerAnimInst->Montage_Play(MontageToPlay);
    }
}

void UCGameplayAbility::StopMontageAfterCurrentSection(UAnimMontage* MontageToStop)
{
    UAnimInstance* OwnerAnimInst = GetOwnerAnimInstance();
    if (OwnerAnimInst)
    {
        FName CurrentSectionName = OwnerAnimInst->Montage_GetCurrentSection(MontageToStop);
        OwnerAnimInst->Montage_SetNextSection(CurrentSectionName, NAME_None, MontageToStop);
    }
}

FGenericTeamId UCGameplayAbility::GetOwnerTeamId() const
{
    IGenericTeamAgentInterface* OwnerTeamInterface = Cast<IGenericTeamAgentInterface>(GetAvatarActorFromActorInfo());
    if (OwnerTeamInterface)
    {
        return OwnerTeamInterface->GetGenericTeamId();
    }

    return FGenericTeamId::NoTeam;
}

bool UCGameplayAbility::IsActorTeamAttitudeIs(const AActor* OtherActor, ETeamAttitude::Type TeamAttitude) const
{
    if (!OtherActor)
        return false;

    IGenericTeamAgentInterface* OwnerTeamAgentInterface = Cast<IGenericTeamAgentInterface>(GetAvatarActorFromActorInfo());
    if (OwnerTeamAgentInterface)
    {
        return OwnerTeamAgentInterface->GetTeamAttitudeTowards(*OtherActor) == TeamAttitude;
    }

    return false;
}

ACharacter* UCGameplayAbility::GetOwningAvatarCharacter()
{
    if (!AvatarCharacter)
    {
        AvatarCharacter = Cast<ACharacter>(GetAvatarActorFromActorInfo());
    }
        
    return AvatarCharacter;
}

void UCGameplayAbility::ApplyGameplayEffectToHitResultActor(const FHitResult& HitResult, TSubclassOf<UGameplayEffect> GameplayEffect, int Level)
{
    FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(GameplayEffect, Level);

    FGameplayEffectContextHandle EffectContext = MakeEffectContext(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo());
    EffectContext.AddHitResult(HitResult);

    EffectSpecHandle.Data->SetContext(EffectContext);

    ApplyGameplayEffectSpecToTarget(GetCurrentAbilitySpecHandle(), CurrentActorInfo, CurrentActivationInfo, EffectSpecHandle, UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActor(HitResult.GetActor()));
}
