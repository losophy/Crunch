// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Minion.h"

void AMinion::SetGenericTeamId(const FGenericTeamId& NewTeamId)
{
	Super::SetGenericTeamId(NewTeamId);
	PickSkinBasedOnTeamID();
}

void AMinion::PickSkinBasedOnTeamID()
{
	USkeletalMesh** Skin = SkinMap.Find(GetGenericTeamId());
	if (Skin)
	{
		GetMesh()->SetSkeletalMesh(*Skin);
	}	
}

void AMinion::OnRep_TeamID()
{
	PickSkinBasedOnTeamID();
}
