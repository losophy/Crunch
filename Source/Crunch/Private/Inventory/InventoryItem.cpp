// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryItem.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffect.h"
#include "GAS/CAbilitySystemStatics.h"
#include "Inventory/PA_ShopItem.h"

FInventoryItemHandle::FInventoryItemHandle()
	: HandleId{ GetInvalidId() }
{
}

FInventoryItemHandle FInventoryItemHandle::InvalidHandle()
{
	static FInventoryItemHandle InvalidHandle = FInventoryItemHandle();
	return InvalidHandle;
}

FInventoryItemHandle::FInventoryItemHandle(uint32 Id)
	: HandleId{ Id }
{
}

FInventoryItemHandle FInventoryItemHandle::CreateHandle()
{
	return FInventoryItemHandle(GenerateNextId());
}

bool FInventoryItemHandle::IsValid() const
{
	return HandleId != GetInvalidId();
}

uint32 FInventoryItemHandle::GenerateNextId()
{
	static uint32 StaticId = 1;
	return StaticId++;
}

uint32 FInventoryItemHandle::GetInvalidId()
{
	return 0;
}

bool operator==(const FInventoryItemHandle& Lhs, const FInventoryItemHandle& Rhs)
{
	return Lhs.GetHandleId() == Rhs.GetHandleId();
}

uint32 GetTypeHash(const FInventoryItemHandle& Key)
{
	return Key.GetHandleId();
}

bool UInventoryItem::AddStackCount()
{
	if (IsStackFull())
	{
		return false;
	}

	++StackCount;
	return true;
}

bool UInventoryItem::ReduceStackCount()
{
	--StackCount;
	if (StackCount <= 0)
	{
		return false;
	}

	return true;
}

bool UInventoryItem::SetStackCount(int NewStackCount)
{
	if (NewStackCount > 0 && NewStackCount <= GetShopItem()->GetMaxStackCount())
	{
		StackCount = NewStackCount;
		return true;
	}

	return false;
}

bool UInventoryItem::IsStackFull() const
{
	return StackCount >= GetShopItem()->GetMaxStackCount();
}

bool UInventoryItem::IsForItem(const UPA_ShopItem* Item) const
{
	if (!Item)
		return false;

	return GetShopItem() == Item;
}

bool UInventoryItem::IsGrantintAbility(TSubclassOf<class UGameplayAbility> AbilityClass) const
{
	if (!ShopItem)
		return false;

	TSubclassOf<UGameplayAbility> GrantedAbility = ShopItem->GetGrantedAbility();
	return GrantedAbility == AbilityClass;
}

bool UInventoryItem::IsGrantingAnyAbility() const
{
	if (!ShopItem)
		return false;

	return ShopItem->GetGrantedAbility() != nullptr;
}

UInventoryItem::UInventoryItem()
	:StackCount{ 1 }
{
}

bool UInventoryItem::IsValid() const
{
	return ShopItem != nullptr;
}

void UInventoryItem::InitItem(const FInventoryItemHandle& NewHandle, const UPA_ShopItem* NewShopItem)
{
	Handle = NewHandle;
	ShopItem = NewShopItem;
}

bool UInventoryItem::TryActivateGrantedAbility(UAbilitySystemComponent* AbilitySystemComponent)
{
	if (!GrantedAbiltiySpecHandle.IsValid())
		return false;

	if (AbilitySystemComponent && AbilitySystemComponent->TryActivateAbility(GrantedAbiltiySpecHandle))
		return true;

	return false;
}

void UInventoryItem::ApplyConsumeEffect(UAbilitySystemComponent* AbilitySystemComponent)
{
	if (!ShopItem)
		return;

	TSubclassOf<UGameplayEffect> ConsumeEffect = ShopItem->GetConsumeEffect();
	if (!ConsumeEffect)
		return;

	AbilitySystemComponent->BP_ApplyGameplayEffectToSelf(ConsumeEffect, 1, AbilitySystemComponent->MakeEffectContext());
}

void UInventoryItem::RemoveGASModifications(UAbilitySystemComponent* AbilitySystemComponent)
{
	if (!AbilitySystemComponent)
		return;

	if (AppliedEquipedEffectHandle.IsValid())
		AbilitySystemComponent->RemoveActiveGameplayEffect(AppliedEquipedEffectHandle);

	if (GrantedAbiltiySpecHandle.IsValid())
		AbilitySystemComponent->SetRemoveAbilityOnEnd(GrantedAbiltiySpecHandle);
}

void UInventoryItem::ApplyGASModifications(UAbilitySystemComponent* AbilitySystemComponent)
{
	if (!GetShopItem() || !AbilitySystemComponent)
		return;

	if (!AbilitySystemComponent->GetOwner() || !AbilitySystemComponent->GetOwner()->HasAuthority())
		return;

	TSubclassOf<UGameplayEffect> EquipEffect = GetShopItem()->GetEquippedEffect();
	if (EquipEffect)
	{
		AppliedEquipedEffectHandle = AbilitySystemComponent->BP_ApplyGameplayEffectToSelf(EquipEffect, 1, AbilitySystemComponent->MakeEffectContext());
	}

	TSubclassOf<UGameplayAbility> GrantedAbility = GetShopItem()->GetGrantedAbility();
	if (GrantedAbility)
	{
		const FGameplayAbilitySpec* FoundSpec = AbilitySystemComponent->FindAbilitySpecFromClass(GrantedAbility);
		if (FoundSpec)
		{
			GrantedAbiltiySpecHandle = FoundSpec->Handle;
		}
		else
		{
			GrantedAbiltiySpecHandle = AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(GrantedAbility));
		}
	}
}

void UInventoryItem::SetSlot(int NewSlot)
{
	Slot = NewSlot;
}
