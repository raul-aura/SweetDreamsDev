// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleState.h"
#include "BattleCharacter.h"

void UBattleState::PostInitProperties()
{
	Super::PostInitProperties();
	Health = BaseHealth;
	Mana = BaseMana;
	Force = BaseForce;
	Resistence = BaseResistence;
	Speed = BaseSpeed;
	HealthPercentage = BaseHealthPercentage;
	ManaPercentage = BaseManaPercentage;
	ForcePercentage = BaseForcePercentage;
	ResistencePercentage = BaseResistencePercentage;
	SpeedPercentage = BaseSpeedPercentage;
	Actions = BaseActions;
	Lives = BaseLives;
}

UObject* UBattleState::GetStateInstigator() const
{
	return StateInstigator;
}

TArray<UObject*> UBattleState::GetInstigatorAsArray() const
{
	TArray<UObject*> ArrayInstigator;
	ArrayInstigator.Add(GetStateInstigator());
	return ArrayInstigator;
}

bool UBattleState::IsTriggerableAction(UBattleAction* Action) const
{
	for (TSubclassOf<UBattleAction> ActionClass : TriggerableActions)
	{
		if (Action->IsA(ActionClass))
		{
			return true;
		}
	}
	return false;
}

void UBattleState::ApplyState(UObject* Instigator)
{
	if (!IsValid(Instigator)) return;
	StateInstigator = Instigator;
	AddStacks();
	ResetLifetime();
	ShowMessage();
	if (bAlreadyAppliedOnce)
	{
		OnReapplied();
		RecalculateParams();
		return;
	}
	OnApplied();
	OnUpdateParams();
	ApplyParams();
	bAlreadyAppliedOnce = true;
}

void UBattleState::ResetLifetime()
{
	IntLifetime = 0;
	FloatLifetime = 0.f;
	switch (Lifetime)
	{
	case EStateLifetime::Turn:
		IntLifetime = TurnsToEnd;
		break;
	case EStateLifetime::Action:
		IntLifetime = ActionsToEnd;
		break;
	case EStateLifetime::Second:
		FloatLifetime = SecondsToEnd;
		break;
	default:
		break;
	}
}

void UBattleState::AddStacks(int32 Amount)
{
	if (!bIsStackable)
	{
		MaxStacks = InitialStacks;
	}
	Stacks = FMath::Clamp(Stacks + Amount, InitialStacks, MaxStacks);
}

void UBattleState::ConsumeStacks(int32 Amount)
{
	if (!bCanConsumeStacks) return;
	Stacks = FMath::Clamp(Stacks - Amount, InitialStacks, MaxStacks);
	if (bRemoveOnZeroStacks && Stacks == 0)
	{
		RemoveState();
	}
}

bool UBattleState::IsMaxStacks() const
{
	return Stacks == MaxStacks;
}

void UBattleState::ConsumeLifetime(EStateLifetime LifetimeToConsume)
{
	if (Lifetime == EStateLifetime::Permanent || LifetimeToConsume != Lifetime) return;
	if (IntLifetime == 0 && FloatLifetime == 0.0f)
	{
		RemoveState();
	}
	switch (Lifetime)
	{
	case EStateLifetime::Turn:
		IntLifetime = FMath::Clamp(--IntLifetime, 0, TurnsToEnd);
		break;
	case EStateLifetime::Action:
		IntLifetime = FMath::Clamp(--IntLifetime, 0, ActionsToEnd);
		break;
	case EStateLifetime::Second:
		FloatLifetime = FMath::Clamp(FloatLifetime - GetWorld()->GetDeltaSeconds(), 0.0f, SecondsToEnd);
		break;
	default:
		break;
	}
	if (bCanConsumeStacksOnLifetime)
	{
		ConsumeStacks();
	}
}

void UBattleState::RemoveState()
{
	UBattlerDataComponent* Data = UBattlerDataComponent::GetBattlerDataComponent(GetOwner());
	if (IsValid(Data))
	{
		Data->RemoveState(GetClass());
	}
}

int32 UBattleState::GetRemainingLifetime() const
{
	return IntLifetime + 1;
}

float UBattleState::GetRemainingLifetimeSeconds() const
{
	return FloatLifetime;
}

void UBattleState::ApplyParams()
{
	if (IsValid(GetOwner()))
	{
		UBattlerDataComponent* Data = UBattlerDataComponent::GetBattlerDataComponent(GetOwner());
		if (IsValid(Data))
		{
			Data->UpdateHealthValue(Health, HealthPercentage);
			Data->UpdateManaValue(Mana, ManaPercentage);
			Data->UpdateForceValue(Force, ForcePercentage);
			Data->UpdateForceMultiplier(ForceMultiplier);
			Data->UpdateResistenceValue(Resistence, ResistencePercentage);
			Data->UpdateResistenceMultiplier(ResistenceMultiplier);
			Data->UpdateSpeedValue(Speed, SpeedPercentage);
			Data->UpdateSpeedMultiplier(SpeedMultiplier);
			Data->UpdateDamageDealtMultiplier(DamageDealtMultiplier);
			Data->UpdateDamageReceivedMultiplier(DamageReceivedMultiplier);
			Data->UpdateHealMultiplier(HealMultiplier);
			Data->UpdateManaRestoreMultiplier(ManaRestoreMultiplier);
			Data->UpdateAdditionalActions(Actions);
			Data->UpdateAdditionalLives(Lives);
		}
	}
}

void UBattleState::RemoveParams()
{
	if (IsValid(GetOwner()))
	{
		UBattlerDataComponent* Data = UBattlerDataComponent::GetBattlerDataComponent(GetOwner());
		if (IsValid(Data))
		{
			Data->UpdateHealthValue(-Health, -HealthPercentage);
			Data->UpdateManaValue(-Mana, -ManaPercentage);
			Data->UpdateForceValue(-Force, -ForcePercentage);
			Data->UpdateForceMultiplier(-ForceMultiplier);
			Data->UpdateResistenceValue(-Resistence, -ResistencePercentage);
			Data->UpdateResistenceMultiplier(-ResistenceMultiplier);
			Data->UpdateSpeedValue(-Speed, -SpeedPercentage);
			Data->UpdateSpeedMultiplier(-SpeedMultiplier);
			Data->UpdateDamageDealtMultiplier(-DamageDealtMultiplier);
			Data->UpdateDamageReceivedMultiplier(-DamageReceivedMultiplier);
			Data->UpdateHealMultiplier(-HealMultiplier);
			Data->UpdateManaRestoreMultiplier(-ManaRestoreMultiplier);
			Data->UpdateAdditionalActions(-Actions);
			Data->UpdateAdditionalLives(-Lives);
		}
	}
}

void UBattleState::RecalculateParams()
{
	RemoveParams();
	OnUpdateParams();
	ApplyParams();
}
