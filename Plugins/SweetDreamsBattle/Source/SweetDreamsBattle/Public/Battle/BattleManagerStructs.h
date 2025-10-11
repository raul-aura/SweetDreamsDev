// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BattleManagerStructs.generated.h"

UENUM(BlueprintType)
enum class ETeamType : uint8
{
	None,
	Player  UMETA(DisplayName = "Player"),
	Hostile UMETA(DisplayName = "Hostile")
};


USTRUCT(BlueprintType)
struct SWEETDREAMSBATTLE_API FBattleActorData
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "Battle Actor Data")
	AActor* BattleActor = nullptr;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Battle Actor Data")
	ETeamType Team = ETeamType::None;

	UPROPERTY(BlueprintReadOnly, Category = "Battle Actor Data")
	float DamageDealt = 0.f;
	UPROPERTY(BlueprintReadOnly, Category = "Battle Actor Data")
	float HealingDealt = 0.f;
	UPROPERTY(BlueprintReadOnly, Category = "Battle Actor Data")
	bool bIsAlive = true;

	FBattleActorData()
		: BattleActor(nullptr),
		Team(ETeamType::None),
		DamageDealt(0.f),
		HealingDealt(0.f),
		bIsAlive(true)
	{}

	FBattleActorData(AActor* InBattleActor)
		: BattleActor(InBattleActor),
		Team(ETeamType::None),
		DamageDealt(0.f),
		HealingDealt(0.f),
		bIsAlive(true)
	{}
};


