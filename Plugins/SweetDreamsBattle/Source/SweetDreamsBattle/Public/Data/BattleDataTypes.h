// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BattleDataTypes.generated.h"

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
			case EParameterModifierType::Multiplier: Multiplier += Mod.Value; break;
			}
		}
		CurrentValue = (BaseValue + Additive + (BaseValue * (Percentage / 100.0f))) * Multiplier;
	}

	FBattleParamater()
		: BaseValue(0.f),
		MinValue(0.f),
		MaxValue(0.f),
		CurrentValue(BaseValue),
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


