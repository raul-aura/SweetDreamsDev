// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Core/SweetDreamsCore.h"
#include "SweetDreamsBPLibrary.generated.h"

class USweetDreamsCore;
class ASweetDreamsGameMode;

UCLASS()
class SWEETDREAMS_API USweetDreamsBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

public:
	static USweetDreamsCore* SweetDreamsCore;
	// DEBUG
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "DreamOrigin", Keywords = "Print SweetDreams core dream debug string text", CallableWithoutWorldContext, DevelopmentOnly), Category = "SweetDreams|Core|DEBUG")
	static void PrintDream(const UObject* DreamOrigin, FString Dream = "Hello dream.", EPrintType Severity = EPrintType::INFO, float duration = 4.0f);
	// GETTERS
	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext))
	static USweetDreamsCore* GetSweetDreamsCore(const UObject* WorldContext);
	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext))
	static ASweetDreamsGameMode* GetSweetDreamsGameMode(const UObject* WorldContext);
	// SAVE
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext))
	static bool CreateSaveGame(const UObject* WorldContext, TSubclassOf<USweetDreamsSaveFile> SaveClass, bool bIsPersistent = true);
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext))
	static bool SaveGame(const UObject* WorldContext, USweetDreamsSaveFile* SaveObject, bool bIsPersistent = true);
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext))
	static USweetDreamsSaveFile* LoadSaveGame(const UObject* WorldContext, bool bIsPersistent = true);
	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext))
	static USweetDreamsSaveFile* GetPersistentSave(const UObject* WorldContext);
	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext))
	static USweetDreamsSaveFile* GetLocalSave(const UObject* WorldContext);
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext))
	static bool DeleteSave(const UObject* WorldContext, bool bIsPersistent = true);
	// SETTINGS
	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext))
	static FDreamUserSettings GetUserSettings(const UObject* WorldContext);
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext))
	static void SetUserSettings(const UObject* WorldContext, FDreamUserSettings Settings);
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext))
	static void SetSettingsQuality(const UObject* WorldContext, int32 Quality);
	// LOADING
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext))
	static void LoadLevel(const UObject* WorldContext, TSoftObjectPtr<UWorld> Level);
	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext))
	static TSoftObjectPtr<UWorld> GetCurrentLoadingLevel(const UObject* WorldContext);
	UFUNCTION(BlueprintPure)
	static float GetLoadingPercentage(TSoftObjectPtr<UObject> Asset);
	// MATH
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext))
	static float IncrementAlpha(const UObject* WorldContext, UPARAM(ref) float& Alpha, float MaxValue = 1.0f, class UCurveFloat* AlphaCurve = nullptr, bool bStopCondition = true);
	UFUNCTION(BlueprintPure, meta = (ExpandBoolAsExecs = "ReturnValue"))
	static bool CalculateChance(float& RandomizedValue, float Chance = 100.f);
	UFUNCTION(BlueprintPure, meta = (ExpandBoolAsExecs = "ReturnValue"))
	static bool CheckInterval(const float& Number, float Interval);
	// HELPERS
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Helpers", meta = (DevelopmentOnly, CompactNodeTitle = "Do Nothing"))
	static void DoNothing() {}
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Helpers", meta = (WorldContext = "WorldContext", CallableWithoutWorldContext, DevelopmentOnly, CompactNodeTitle = "Should Not Happen"))
	static void ShouldNotHappen(const UObject* WorldContext);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Helpers", meta = (WorldContext = "WorldContext", CallableWithoutWorldContext, DevelopmentOnly, CompactNodeTitle = "Do Something"))
	static void DoSomething(const UObject* WorldContext);
	UFUNCTION(BlueprintPure, Category = "Sweet Dreams|Core|Helpers")
	static bool IsRunningInEditor();
	UFUNCTION(BlueprintPure, Category = "Sweet Dreams|Core|Helpers")
	static bool IsRunningInStandaloneGame();
};
