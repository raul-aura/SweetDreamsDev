// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BattleDataTypes.generated.h"

class UBattleEvent;

UENUM(BlueprintType)
enum class EBattleElementEndMode : uint8
{
	Auto,      
	Manual  
};

UENUM(BlueprintType)
enum class ETeamType : uint8
{
	None,
	Player,
	Hostile
};

UENUM(BlueprintType)
enum class EParameterModifierType : uint8
{
	Absolute,
	Percentage,
	Multiplier
};

USTRUCT(BlueprintType)
struct SWEETDREAMSBATTLE_API FBattleParameterModifier
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Battle Parameter")
	EParameterModifierType ModifierType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Battle Parameter")
	float Value;

	FBattleParameterModifier()
		: ModifierType(EParameterModifierType::Absolute),
		Value(0.f)
	{}

	FBattleParameterModifier(EParameterModifierType InType, float InValue)
		: ModifierType(InType),
		Value(InValue)
	{}

	bool operator==(const FBattleParameterModifier& Other) const
	{
		return ModifierType == Other.ModifierType && FMath::IsNearlyEqual(Value, Other.Value);
	}
};


USTRUCT(BlueprintType)
struct SWEETDREAMSBATTLE_API FBattleParamater
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Battle Parameter")
	float BaseValue;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Battle Parameter")
	float MinValue;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Battle Parameter")
	float MaxValue;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Battle Parameter")
	float CurrentValue;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Battle Parameter")
	TArray<FBattleParameterModifier> Modifiers;

	void AddModifier(const FBattleParameterModifier& Modifier)
	{
		Modifiers.Add(Modifier);
		EvaluateModifiers();
	}

	void RemoveModifier(const FBattleParameterModifier& Modifier)
	{
		if (Modifiers.Contains(Modifier))
		{
			Modifiers.Remove(Modifier);
			EvaluateModifiers();
		}
	}
	
	void EvaluateModifiers() 
	{
		float Additive = 0.f;
		float Percentage = 0.f;
		float Multiplier = 1.f;
		for (const FBattleParameterModifier& Mod : Modifiers)
		{
			switch (Mod.ModifierType)
			{
			case EParameterModifierType::Absolute:   Additive += Mod.Value; break;
			case EParameterModifierType::Percentage: Percentage += Mod.Value; break;
			case EParameterModifierType::Multiplier: Multiplier *= Mod.Value; break;
			}
		}
		CurrentValue = (BaseValue + Additive + (BaseValue * (Percentage / 100.0f))) * Multiplier;

		if (MaxValue >= 0)
		{
			CurrentValue = FMath::Clamp(CurrentValue, MinValue, MaxValue);
		}
		else
		{
			CurrentValue = FMath::Max(CurrentValue, MinValue);
		}
	}

	FBattleParamater()
		: BaseValue(0.f),
		MinValue(0.f),
		MaxValue(-1.f),
		CurrentValue(0.f),
		Modifiers(TArray<FBattleParameterModifier>())
	{}

	FBattleParamater(float InValue)
		: BaseValue(InValue),
		MinValue(0.f),
		MaxValue(InValue),
		CurrentValue(InValue),
		Modifiers(TArray<FBattleParameterModifier>())
	{}
};

USTRUCT(BlueprintType)
struct SWEETDREAMSBATTLE_API FBattleHealth
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Health")
	FBattleParamater MaxHealth;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Health")
	float CurrentHealth;

	void Initialize()
	{
		MaxHealth.EvaluateModifiers();
		CurrentHealth = MaxHealth.CurrentValue;
	}

	void Damage(float Value)
	{
		if (Value <= 0.f) return;

		CurrentHealth = FMath::Clamp(CurrentHealth - Value, 0.f, MaxHealth.CurrentValue);
	}

	void Heal(float Value)
	{
		if (Value <= 0.f) return;

		CurrentHealth = FMath::Clamp(CurrentHealth + Value, 0.f, MaxHealth.CurrentValue);
	}

	void OnMaxHealthChanged()
	{
		CurrentHealth = FMath::Clamp(CurrentHealth, 0.f, MaxHealth.CurrentValue);
	}

	void OnMaxHealthChangedPreserveRatio(float OldMaxHealth)
	{
		if (OldMaxHealth <= 0.f)
		{
			CurrentHealth = MaxHealth.CurrentValue;
			return;
		}

		const float Ratio = CurrentHealth / OldMaxHealth;

		CurrentHealth = FMath::Clamp(Ratio * MaxHealth.CurrentValue,0.f,MaxHealth.CurrentValue);
	}

	float GetHealthPercent() const
	{
		return MaxHealth.CurrentValue > 0.f ? CurrentHealth / MaxHealth.CurrentValue : 0.f;
	}

	FBattleHealth()
		: CurrentHealth(0.f)
	{}

	FBattleHealth(float InBaseHealth)
		: MaxHealth(InBaseHealth),
		CurrentHealth(InBaseHealth)
	{
		MaxHealth.EvaluateModifiers();
	}
};

