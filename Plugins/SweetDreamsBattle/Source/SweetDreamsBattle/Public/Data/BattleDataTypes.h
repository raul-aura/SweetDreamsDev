// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/BattleParameterData.h"
#include "GameplayTagContainer.h"
#include "BattleDataTypes.generated.h"

class UBattleEvent;
class UBattleParameterData;
class UBattleActorComponent;
class UBattleParameterEvent;

UENUM(BlueprintType)
enum class ETargetSelectionScope : uint8
{
	Candidates,
	CandidatesAndOwner UMETA(DisplayName = "Candidates and Owner"),
	OwnerOnly UMETA(DisplayName = "Owner Only")
};

USTRUCT(BlueprintType)
struct SWEETDREAMSBATTLE_API FSelectedTargetsSettings
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Selected Targets Settings")
	ETargetSelectionScope TargetType = ETargetSelectionScope::Candidates;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Selected Targets Settings", meta = (EditCondition = "TargetType!=ETargetSelectionScope::OwnerOnly", EditConditionHides))
	int32 MaxAmount = 0;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Selected Targets Settings", meta = (EditCondition = "TargetType!=ETargetSelectionScope::OwnerOnly", EditConditionHides))
	bool bRandomizeSelection = false;

	FSelectedTargetsSettings() = default;
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
	EParameterModifierType ModifierType = EParameterModifierType::Absolute;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Battle Parameter")
	float Value = 0.f;

	FBattleParameterModifier() = default;

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
struct SWEETDREAMSBATTLE_API FBattleParameter
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Battle Parameter")
	TObjectPtr<UBattleParameterData> Data;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Battle Parameter")
	TArray<FBattleParameterModifier> Modifiers;

	UPROPERTY(BlueprintReadOnly, Category = "Battle Parameter")
	TObjectPtr<UBattleParameterEvent> Event;

	void InitializeParameter()
	{
		if (Data)
		{
			ParameterValue = GetUpdatedParameterValue();
			ResourceValue = ParameterValue * Data->InitialResourceValueRatio;
		}
	}

	void AddModifier(const FBattleParameterModifier& Modifier)
	{
		Modifiers.Add(Modifier);
		UpdateParameter();
	}

	void RemoveModifier(const FBattleParameterModifier& Modifier)
	{
		if (Modifiers.Contains(Modifier))
		{
			Modifiers.Remove(Modifier);
			UpdateParameter();
		}
	}

	void UpdateParameter()
	{
		const float PreviousParameterValue = ParameterValue;

		ParameterValue = GetUpdatedParameterValue();
		ResourceValue = GetUpdatedResourceValue(PreviousParameterValue);
	}
	
	float GetUpdatedParameterValue() const
	{
		if (!Data)
		{
			return 0.f;
		}

		float Value = Data->InitialValue;

		if (!Modifiers.IsEmpty())
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

			Value = (Data->InitialValue + Additive + (Data->InitialValue * (Percentage / 100.0f))) * Multiplier;
		}

		if (Data->bUseMinValue)
		{
			Value = FMath::Max(Value, Data->MinValue);
		}

		if (Data->bUseMaxValue)
		{
			Value = FMath::Min(Value, Data->MaxValue);
		}

		return Value;
	}

	float GetUpdatedResourceValue(const float OldMaxValue) const
	{
		if (!Data || !Data->bIsResource) return 0.f;

		const float NewMax = GetUpdatedParameterValue();
		float Value = ResourceValue;

		if (Data->bPreserveResourceRatioOnUpdate && OldMaxValue > 0.f)
		{
			const float Ratio = ResourceValue / OldMaxValue;
			Value = Ratio * NewMax;
		}

		if (Data->bUseMinValue)
		{
			Value = FMath::Max(Value, Data->MinValue);
		}

		return FMath::Min(Value, NewMax);
	}

	void AddResourceValue(const float Value)
	{
		ResourceValue += Value;
		ClampResourceValue();
	}

	void RemoveResourceValue(const float Value)
	{
		ResourceValue -= Value;
		ClampResourceValue();
	}

	void ClampResourceValue()
	{
		if (Data && Data->bUseMinValue)
		{
			ResourceValue = FMath::Max(ResourceValue, Data->MinValue);
		}

		ResourceValue = FMath::Min(ResourceValue, ParameterValue);
	}

	float GetParameterValue() const
	{
		return ParameterValue;
	}

	float GetResourceValue() const
	{
		return ResourceValue;
	}

	FBattleParameter()
		: Data(nullptr),
		Modifiers(TArray<FBattleParameterModifier>()),
		Event(nullptr),
		ParameterValue(0.f),
		ResourceValue(0.f)
	{}

private:

	float ParameterValue = 0.f;
	float ResourceValue = 0.f;
};

