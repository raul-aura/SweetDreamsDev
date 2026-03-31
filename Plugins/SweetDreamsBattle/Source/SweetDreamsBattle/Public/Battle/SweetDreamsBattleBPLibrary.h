// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Data/BattleDataTypes.h"
#include "SweetDreamsBattleBPLibrary.generated.h"

UCLASS()
class USweetDreamsBattleBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

public:

	UFUNCTION(BlueprintPure, Category = "Sweet Dreams|RPG", meta = (WorldContext = "WorldContext", CallableWithoutWorldContext))
	static USweetDreamsBattleCore* GetSweetDreamsBattleCore(const UObject* WorldContext);

	// BATTLE
	UFUNCTION(BlueprintPure, Category = "Sweet Dreams|RPG")
	static bool AreTeamsHostile(const ETeamType TeamA, const ETeamType TeamB);
	UFUNCTION(BlueprintPure, Category = "Sweet Dreams|RPG")
	static TArray<UBattleActorComponent*> GetTargetsFromDamageHealResult(const FDamageHealResult& Result);

	// DIFFICULTY
	UFUNCTION(BlueprintPure, Category = "Sweet Dreams|RPG", meta = (WorldContext = "WorldContext", CallableWithoutWorldContext))
	static int32 GetDifficulty(const UObject* WorldContext);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG", meta = (WorldContext = "WorldContext", CallableWithoutWorldContext))
	static void SetDifficulty(const UObject* WorldContext, int32 NewDifficulty = 1);

	// LEVEL
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG", meta = (WorldContext = "WorldContext", CallableWithoutWorldContext))
	static int32 UpdateLevelsByAverage(const UObject* WorldContext, const TArray<AActor*>& TargetActors, const TArray<AActor*>& AvarageActors);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG", meta = (WorldContext = "WorldContext", CallableWithoutWorldContext))
	static int32 GetAverageLevel(const UObject* WorldContext, const TArray<AActor*>& Actors);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG", meta = (WorldContext = "WorldContext", CallableWithoutWorldContext))
	static void OverrideLevels(const UObject* WorldContext, const TArray<AActor*>& Actors, int32 NewLevel = 1);

	// PARAM MATH
	UFUNCTION(BlueprintPure, Category = "Sweet Dreams|RPG")
	static float IncreaseParameterLinear(UPARAM(ref)float& Parameter, float BaseParameter, float AdditionalParameter, float Multiplier);
	UFUNCTION(BlueprintPure, Category = "Sweet Dreams|RPG")
	static float IncreaseParameterExponential(UPARAM(ref)float& Parameter, float BaseParameter, float AdditionalParameter, float Multiplier, float Power = 2.f);
	UFUNCTION(BlueprintPure, Category = "Sweet Dreams|RPG")
	static float IncreaseParameterLogarithmic(UPARAM(ref)float& Parameter, float BaseParameter, float AdditionalParameter, float Multiplier, float LogBase = 2.f);
	UFUNCTION(BlueprintPure, Category = "Sweet Dreams|RPG")
	static float IncreaseParameterDiminishing(UPARAM(ref)float& Parameter, float BaseParameter, float AdditionalParameter, float Multiplier, float DiminishFactor = 0.1f);
};
