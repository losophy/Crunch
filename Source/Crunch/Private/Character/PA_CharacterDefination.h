// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PA_CharacterDefination.generated.h"

class ACCharacter;
/**
 * 
 */
UCLASS()
class UPA_CharacterDefination : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
	static FPrimaryAssetType GetCharacterDefinationAssetType();

	FString GEtCharacterDisplayName() const { return CharacterName; }
	UTexture2D* LoadIcon() const;
	TSubclassOf<ACCharacter> LoadCharacterClass() const;
	TSubclassOf<UAnimInstance> LoadDisplayAnimationBP() const;
	class USkeletalMesh* LoadDisplayMesh() const;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Character")
	FString CharacterName;

	UPROPERTY(EditDefaultsOnly, Category = "Character")
	TSoftObjectPtr<UTexture2D> CharacterIcon;

	UPROPERTY(EditDefaultsOnly, Category = "Character")
	TSoftClassPtr<ACCharacter> CharacterClass;

	UPROPERTY(EditDefaultsOnly, Category = "Character")
	TSoftClassPtr<UAnimInstance> DisplayAnimBP;
};
