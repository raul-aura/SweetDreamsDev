// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Data/BattleDataTypes.h"
#include "SweetDreamsBattleBPLibrary.generated.h"

class USweetDreamsBattleCore;
class UBattleElement;

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

	// ELEMENTS
	UFUNCTION(BlueprintPure, Category = "Sweet Dreams|RPG")
	static UBattleElementData* GetRandomBattleElement(const TArray<UBattleElement*>& Elements);

	// DIFFICULTY
	UFUNCTION(BlueprintPure, Category = "Sweet Dreams|RPG", meta = (WorldContext = "WorldContext", CallableWithoutWorldContext))
	static int32 GetDifficulty(const UObject* WorldContext);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG", meta = (WorldContext = "WorldContext", CallableWithoutWorldContext))
	static void SetDifficulty(const UObject* WorldContext, int32 NewDifficulty = 1);
};
