// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/PA_AbilitySystemGenerics.h"

const FRealCurve* UPA_AbilitySystemGenerics::GetExperienceCurve() const
{
    return ExperienceCurveTable->FindCurve(ExperienceRowName, "");
}
