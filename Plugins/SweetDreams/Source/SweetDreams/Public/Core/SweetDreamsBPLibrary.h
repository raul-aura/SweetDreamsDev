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
	// DEBUG
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "DreamOrigin", CallableWithoutWorldContext, DevelopmentOnly), Category = "SweetDreams|Core|DEBUG")
	static void PrintDream(const UObject* DreamOrigin, FString Dream = "Hello dream.", EPrintType Severity = EPrintType::INFO, float duration = 4.0f);
	// GETTERS
	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext), Category = "Sweet Dreams|Core")
	static USweetDreamsCore* GetSweetDreamsCore(const UObject* WorldContext);
	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext), Category = "Sweet Dreams|Core")
	static ASweetDreamsGameMode* GetSweetDreamsGameMode(const UObject* WorldContext);
	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext), Category = "Sweet Dreams|Core")
	static FString GetGameVersion(const UObject* WorldContext);
	// SAVE
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext), Category = "Sweet Dreams|Core|Save")
	static USweetDreamsSaveFile* CreateCustomSave(const UObject* WorldContext, TSubclassOf<USweetDreamsSaveFile> SaveClass, FString CustomSlot, bool& bSuccessful);
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext), Category = "Sweet Dreams|Core|Save")
	static USweetDreamsSaveFile* CreatePersistentSave(const UObject* WorldContext, TSubclassOf<USweetDreamsSaveFile> SaveClass, bool& bSuccessful); //overriding auto save 
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext), Category = "Sweet Dreams|Core|Save")
	static USweetDreamsSaveFile* CreateLocalSave(const UObject* WorldContext, TSubclassOf<USweetDreamsSaveFile> SaveClass, bool& bSuccessful); //overriding auto save
	//
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext), Category = "Sweet Dreams|Core|Save")
	static bool SaveCustomGame(const UObject* WorldContext, FString CustomSlot);
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext), Category = "Sweet Dreams|Core|Save")
	static bool SavePersistentGame(const UObject* WorldContext);
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext), Category = "Sweet Dreams|Core|Save")
	static bool SaveLocalGame(const UObject* WorldContext);
	//
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext), Category = "Sweet Dreams|Core|Save")
	static USweetDreamsSaveFile* LoadCustomGame(const UObject* WorldContext, FString CustomSlot);
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext), Category = "Sweet Dreams|Core|Save")
	static USweetDreamsSaveFile* LoadPersistentGame(const UObject* WorldContext);
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext), Category = "Sweet Dreams|Core|Save")
	static USweetDreamsSaveFile* LoadLocalGame(const UObject* WorldContext);
	//
	static bool DeleteCustomGame(const UObject* WorldContext, FString CustomSlot);
	static bool DeletePersistentGame(const UObject* WorldContext);
	static bool DeleteLocalGame(const UObject* WorldContext);
	//
	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext), Category = "Sweet Dreams|Core|Save")
	static USweetDreamsSaveFile* GetCustomSave(const UObject* WorldContext, FString Slot);
	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext), Category = "Sweet Dreams|Core|Save")
	static USweetDreamsSaveFile* GetPersistentSave(const UObject* WorldContext);
	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext), Category = "Sweet Dreams|Core|Save")
	static USweetDreamsSaveFile* GetLocalSave(const UObject* WorldContext);
	// SETTINGS
	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext), Category = "Sweet Dreams|Core|Settings")
	static FDreamUserSettings GetUserSettings(const UObject* WorldContext);
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext), Category = "Sweet Dreams|Core|Settings")
	static void SetUserSettings(const UObject* WorldContext, FDreamUserSettings Settings);
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext), Category = "Sweet Dreams|Core|Settings")
	static void SetSettingsQuality(const UObject* WorldContext, int32 Quality);
	// LOADING
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext), Category = "Sweet Dreams|Core|Loading")
	static void LoadLevel(const UObject* WorldContext, TSoftObjectPtr<UWorld> Level);
	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext), Category = "Sweet Dreams|Core|Loading")
	static TSoftObjectPtr<UWorld> GetCurrentLoadingLevel(const UObject* WorldContext);
	UFUNCTION(BlueprintPure, Category = "Sweet Dreams|Core|Loading")
	static float GetAssetLoadingPercentage(TSoftObjectPtr<UObject> Asset);
	// MATH
	UFUNCTION(BlueprintPure, meta = (ExpandBoolAsExecs = "ReturnValue"), Category = "Sweet Dreams|Core|Math")
	static bool CalculateChance(float& RandomizedValue, float Chance = 100.f);
	UFUNCTION(BlueprintPure, meta = (ExpandBoolAsExecs = "ReturnValue"), Category = "Sweet Dreams|Core|Math")
	static bool IsMultipleOf(const float& Number, float Interval, float Tolerance = 0.01f);
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
	//
	static inline UWorld* GetValidWorld(const UObject* WorldContext)
	{
		if (!IsValid(WorldContext)) return nullptr;
		return GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::ReturnNull);
	}
};
