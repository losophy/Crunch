// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/CGameplayAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Components/SkeletalMeshComponent.h"
#include "GAS/GAP_Launched.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"

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

ACharacter* UCGameplayAbility::GetOwningAvatarCharacter()
{
    if (!AvatarCharacter)
    {
        AvatarCharacter = Cast<ACharacter>(GetAvatarActorFromActorInfo());
    }
        
    return AvatarCharacter;
}
