// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Core/SweetDreamsCore.h"
#include "FindSessionsCallbackProxy.h"
#include "SweetDreamsBPLibrary.generated.h"

class USweetDreamsCore;
class ASweetDreamsGameMode;
struct FBlueprintSessionResult;

UCLASS()
class SWEETDREAMSFREE_API USweetDreamsBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

public:
	// DEBUG
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "DreamOrigin", CallableWithoutWorldContext, DevelopmentOnly), Category = "Sweet Dreams|Core")
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
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext), Category = "Sweet Dreams|Core|Save")
	static bool SaveCustomGame(const UObject* WorldContext, FString CustomSlot);
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext), Category = "Sweet Dreams|Core|Save")
	static bool SavePersistentGame(const UObject* WorldContext);
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext), Category = "Sweet Dreams|Core|Save")
	static bool SaveLocalGame(const UObject* WorldContext);
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext), Category = "Sweet Dreams|Core|Save")
	static USweetDreamsSaveFile* LoadCustomGame(const UObject* WorldContext, FString CustomSlot);
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext), Category = "Sweet Dreams|Core|Save")
	static USweetDreamsSaveFile* LoadPersistentGame(const UObject* WorldContext);
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext), Category = "Sweet Dreams|Core|Save")
	static USweetDreamsSaveFile* LoadLocalGame(const UObject* WorldContext);
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext), Category = "Sweet Dreams|Core|Save")
	static bool DeleteCustomGame(const UObject* WorldContext, FString CustomSlot);
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext), Category = "Sweet Dreams|Core|Save")
	static bool DeletePersistentGame(const UObject* WorldContext);
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext), Category = "Sweet Dreams|Core|Save")
	static bool DeleteLocalGame(const UObject* WorldContext);
	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext), Category = "Sweet Dreams|Core|Save")
	static USweetDreamsSaveFile* GetCustomSave(const UObject* WorldContext, FString Slot);
	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext), Category = "Sweet Dreams|Core|Save")
	static USweetDreamsSaveFile* GetPersistentSave(const UObject* WorldContext);
	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext), Category = "Sweet Dreams|Core|Save")
	static USweetDreamsSaveFile* GetLocalSave(const UObject* WorldContext);

	// WORLD
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext), Category = "Sweet Dreams|Core|World")
	static void PauseActors(UObject* WorldContext, TArray<AActor*> InActors);
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext), Category = "Sweet Dreams|Core|World")
	static void PauseAllActors(UObject* WorldContext, TSubclassOf<AActor> IgnoreActorClass = nullptr);
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext), Category = "Sweet Dreams|Core|World")
	static void ResumeActors(UObject* WorldContext, TArray<AActor*> InActors);
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext), Category = "Sweet Dreams|Core|World")
	static void ResumeAllActors(UObject* WorldContext, TSubclassOf<AActor> IgnoreActorClass = nullptr);
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext), Category = "Sweet Dreams|Core|World")
	static void SetTimeDilationActors(UObject* WorldContext, TArray<AActor*> InActors, const float NewTimeDilation = 0.f, TSubclassOf<AActor> IgnoreActorClass = nullptr);

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

	// REPLICATION
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Replication", meta = (WorldContext = "WorldContext"))
	static void ServerTravel(UObject* WorldContext, FString MapName, bool bIsListenServer, bool bAbsolute);
	UFUNCTION(BlueprintPure, Category = "Sweet Dreams|Core|Replication")
	static bool GetSessionSetting(FBlueprintSessionResult Result, FName Key, FString& Value);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Replication")
	static void SetSessionSetting(FBlueprintSessionResult Result, FName Key, const FString& Value);
	static FNamedOnlineSession* GetCurrentSession(UObject* WorldContext);
	UFUNCTION(BlueprintPure, Category = "Sweet Dreams|Core|Replication", meta = (WorldContext = "WorldContext"))
	static bool GetCurrentSessionSetting(UObject* WorldContext, FName Key, FString& Value);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Replication", meta = (WorldContext = "WorldContext"))
	static void SetCurrentSessionSetting(UObject* WorldContext, FName Key, const FString& Value);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Replication", meta = (WorldContext = "WorldContext"))
	static void RenameSession(UObject* WorldContext, const FString& NewName, int32 MaxLength = 32);
	UFUNCTION(BlueprintPure, Category = "Sweet Dreams|Core|Replication", meta = (WorldContext = "WorldContext"))
	static bool GetCurrentSessionName(UObject* WorldContext, FString& SessionName);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Replication", meta = (WorldContext = "WorldContext"))
	static void SetSessionPlayerReady(UObject* WorldContext, const FString& PlayerID, bool bReady);
	UFUNCTION(BlueprintPure, Category = "Sweet Dreams|Core|Replication", meta = (WorldContext = "WorldContext"))
	static bool IsPlayerReady(UObject* WorldContext, const FString& PlayerID);
	UFUNCTION(BlueprintPure, Category = "Sweet Dreams|Core|Replication", meta = (WorldContext = "WorldContext"))
	static bool ArePlayersReady(UObject* WorldContext);
	UFUNCTION(BlueprintPure, Category = "Sweet Dreams|Core|Replication", meta = (WorldContext = "WorldContext"))
	static FString GetPlayerUniqueId(APlayerController* PlayerController);

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
	static inline UWorld* GetValidWorld(const UObject* WorldContext)
	{
		if (!IsValid(WorldContext)) return nullptr;
		return GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::ReturnNull);
	}
};
