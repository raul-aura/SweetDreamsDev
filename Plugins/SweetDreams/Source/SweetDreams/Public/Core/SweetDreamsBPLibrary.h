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
class SWEETDREAMS_API USweetDreamsBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

public:
	// DEBUG

	// Prints a string to the Output Log and screen, the string color is based on the print severity.
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "DreamOrigin", CallableWithoutWorldContext, DevelopmentOnly), Category = "Sweet Dreams|Core")
	static void PrintDream(const UObject* DreamOrigin, FString Dream = "Hello dream.", EPrintType Severity = EPrintType::INFO, float duration = 4.0f);

	// GETTERS

	// Gets the SweetDreamsCore GameInstance Subsystem.
	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext), Category = "Sweet Dreams|Core")
	static USweetDreamsCore* GetSweetDreamsCore(const UObject* WorldContext);
	// Attempts to get the GameMode cast to SweetDreamsGameMode.
	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext), Category = "Sweet Dreams|Core")
	static ASweetDreamsGameMode* GetSweetDreamsGameMode(const UObject* WorldContext);
	// Gets the Game Version set on Project Settings > Sweet Dreams Core > Game Version.
	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext), Category = "Sweet Dreams|Core")
	static FString GetGameVersion(const UObject* WorldContext);

	// SAVE
	// Creates a custom Save Object using the specified class and stores in the CustomSaves array of the SweetDreamsCore Subsystem.
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext), Category = "Sweet Dreams|Core|Save")
	static USweetDreamsSaveFile* CreateCustomSave(const UObject* WorldContext, TSubclassOf<USweetDreamsSaveFile> SaveClass, FString CustomSlot, bool& bSuccessful);
	// Manually creates a Persistent Save Object using the specified class. Persistent Saves are not destroyed upon game shutdown.
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext), Category = "Sweet Dreams|Core|Save")
	static USweetDreamsSaveFile* CreatePersistentSave(const UObject* WorldContext, TSubclassOf<USweetDreamsSaveFile> SaveClass, bool& bSuccessful); 
	// Manually creates a Local Save Object using the specified class. Local Saves are destroyed upon game shutdown.
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext), Category = "Sweet Dreams|Core|Save")
	static USweetDreamsSaveFile* CreateLocalSave(const UObject* WorldContext, TSubclassOf<USweetDreamsSaveFile> SaveClass, bool& bSuccessful); 
	// Saves a custom Game Object to disk if found on the SweetDreamsCore Subsystem using the custom slot.
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext), Category = "Sweet Dreams|Core|Save")
	static bool SaveCustomGame(const UObject* WorldContext, FString CustomSlot);
	// Saves the Persistent Save to disk.
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext), Category = "Sweet Dreams|Core|Save")
	static bool SavePersistentGame(const UObject* WorldContext);
	// Saves the Local Save to disk.
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext), Category = "Sweet Dreams|Core|Save")
	static bool SaveLocalGame(const UObject* WorldContext);
	// Loads a custom Save from disk and updates the variable on the SweetDreamsCore Subsystem if found using the custom slot.
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext), Category = "Sweet Dreams|Core|Save")
	static USweetDreamsSaveFile* LoadCustomGame(const UObject* WorldContext, FString CustomSlot);
	// Loads the Persistent Save file from disk.
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext), Category = "Sweet Dreams|Core|Save")
	static USweetDreamsSaveFile* LoadPersistentGame(const UObject* WorldContext);
	// Loads the Local Save file from disk.
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext), Category = "Sweet Dreams|Core|Save")
	static USweetDreamsSaveFile* LoadLocalGame(const UObject* WorldContext);
	// Deletes a custom Save Object from disk if found using the custom slot.
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext), Category = "Sweet Dreams|Core|Save")
	static bool DeleteCustomGame(const UObject* WorldContext, FString CustomSlot);
	// Deletes the Persistent Save file from disk.
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext), Category = "Sweet Dreams|Core|Save")
	static bool DeletePersistentGame(const UObject* WorldContext);
	// Deletes the Local Save file from disk.
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext), Category = "Sweet Dreams|Core|Save")
	static bool DeleteLocalGame(const UObject* WorldContext);
	// Gets the custom Save Object from the SweetDreamsCore Subsystem using the custom slot.
	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext), Category = "Sweet Dreams|Core|Save")
	static USweetDreamsSaveFile* GetCustomSave(const UObject* WorldContext, FString Slot);
	// Gets the saved Persistent Save Object from the SweetDreamsCore Subsystem.
	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext), Category = "Sweet Dreams|Core|Save")
	static USweetDreamsSaveFile* GetPersistentSave(const UObject* WorldContext);
	// Gets the saved Local Save Object from the SweetDreamsCore Subsystem.
	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext), Category = "Sweet Dreams|Core|Save")
	static USweetDreamsSaveFile* GetLocalSave(const UObject* WorldContext);

	// WORLD

	// Gets all actors from the array and sets their Time Dilation to 0.
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext), Category = "Sweet Dreams|Core|World")
	static void PauseActors(UObject* WorldContext, TArray<AActor*> InActors);
	// Gets all actors from the current level and sets their Time Dilation to 0.
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext), Category = "Sweet Dreams|Core|World")
	static void PauseAllActors(UObject* WorldContext, TSubclassOf<AActor> IgnoreActorClass = nullptr);
	// Gets all actors from the array and sets their Time Dilation to 1.
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext), Category = "Sweet Dreams|Core|World")
	static void ResumeActors(UObject* WorldContext, TArray<AActor*> InActors);
	// Gets all actors from the current level and sets their Time Dilation to 1.
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext), Category = "Sweet Dreams|Core|World")
	static void ResumeAllActors(UObject* WorldContext, TSubclassOf<AActor> IgnoreActorClass = nullptr);
	// Gets all actors from the array, excluding actors from the specified class to ignore, and sets their Time Dilation to a specified number.
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext), Category = "Sweet Dreams|Core|World")
	static void SetTimeDilationActors(UObject* WorldContext, TArray<AActor*> InActors, const float NewTimeDilation = 0.f, TSubclassOf<AActor> IgnoreActorClass = nullptr);

	// LOADING

	// Loads a level through the SweetDreamsCore subsystem.
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext), Category = "Sweet Dreams|Core|Loading")
	static void LoadLevel(const UObject* WorldContext, TSoftObjectPtr<UWorld> Level);
	// Gets the current loading level from the SweetDreamsCore subsystem, if any.
	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext), Category = "Sweet Dreams|Core|Loading")
	static TSoftObjectPtr<UWorld> GetCurrentLoadingLevel(const UObject* WorldContext);
	// Returns the value of loading from an asset, between 0 and 100. Will return 100 if the asset is invalid or not loading.
	UFUNCTION(BlueprintPure, Category = "Sweet Dreams|Core|Loading")
	static float GetAssetLoadingPercentage(TSoftObjectPtr<UObject> Asset);

	// MATH

	// Returns true if the random number is equal or less than the specified chance normalized.
	UFUNCTION(BlueprintPure, meta = (ExpandBoolAsExecs = "ReturnValue"), Category = "Sweet Dreams|Core|Math")
	static bool CalculateChance(float& RandomizedValue, float Chance = 100.f);
	// Returns true if the number is multiple of the interval, accepting a tolerance.
	UFUNCTION(BlueprintPure, meta = (ExpandBoolAsExecs = "ReturnValue"), Category = "Sweet Dreams|Core|Math")
	static bool IsMultipleOf(const float Number, const float Interval, const float Tolerance = 0.01f);

	// REPLICATION

	// Loads and travels to another level, taking all connected clients to the server with it.
	// This can only be called on the server.
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Replication", meta = (WorldContext = "WorldContext"))
	static void ServerTravel(UObject* WorldContext, FString MapName, bool bIsListenServer, bool bAbsolute);
	// Gets a custom setting from the Online Session.
	UFUNCTION(BlueprintPure, Category = "Sweet Dreams|Core|Replication")
	static bool GetSessionSetting(FBlueprintSessionResult Result, FName Key, FString& Value);
	// Sets a custom setting from the Online Session.
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Replication")
	static void SetSessionSetting(FBlueprintSessionResult Result, FName Key, const FString& Value);
	static FNamedOnlineSession* GetCurrentSession(UObject* WorldContext);
	// Gets a custom setting from the current Online Session, detecting it automatically if any.
	UFUNCTION(BlueprintPure, Category = "Sweet Dreams|Core|Replication", meta = (WorldContext = "WorldContext"))
	static bool GetCurrentSessionSetting(UObject* WorldContext, FName Key, FString& Value);
	// Sets a custom setting from the current Online Session, detecting it automatically if any.
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Replication", meta = (WorldContext = "WorldContext"))
	static void SetCurrentSessionSetting(UObject* WorldContext, FName Key, const FString& Value);
	// Renames the current Online Session, changing the value of the custom setting "SERVER_NAME".
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Replication", meta = (WorldContext = "WorldContext"))
	static void RenameSession(UObject* WorldContext, const FString& NewName, int32 MaxLength = 32);
	// Gets the value of the custom setting "SERVER_NAME" from the current Online Session. Returns true if found Online Session.
	UFUNCTION(BlueprintPure, Category = "Sweet Dreams|Core|Replication", meta = (WorldContext = "WorldContext"))
	static bool GetCurrentSessionName(UObject* WorldContext, FString& SessionName);
	// Set the value as "true/false" for the "PLAYER_READY_PlayerID" custom setting from the current Online Session.
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Replication", meta = (WorldContext = "WorldContext"))
	static void SetSessionPlayerReady(UObject* WorldContext, const FString& PlayerID, bool bReady);
	// Returns the value for the "PLAYER_READY_PlayerID" custom setting from the current Online Session.
	UFUNCTION(BlueprintPure, Category = "Sweet Dreams|Core|Replication", meta = (WorldContext = "WorldContext"))
	static bool IsPlayerReady(UObject* WorldContext, const FString& PlayerID);
	// Returns true if all players registered in the current Online Session have their "PLAYER_READY_PlayerID" value as true.
	UFUNCTION(BlueprintPure, Category = "Sweet Dreams|Core|Replication", meta = (WorldContext = "WorldContext"))
	static bool ArePlayersReady(UObject* WorldContext);
	// Returns the PlayerID directly from the Player Controller.
	UFUNCTION(BlueprintPure, Category = "Sweet Dreams|Core|Replication", meta = (WorldContext = "WorldContext"))
	static FString GetPlayerUniqueId(APlayerController* PlayerController);

	// HELPERS

	// Empty function for debugging.
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Helpers", meta = (DevelopmentOnly, CompactNodeTitle = "Do Nothing"))
	static void DoNothing() {}
	// Debug function that will print "Should NOT happen called."
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Helpers", meta = (WorldContext = "WorldContext", CallableWithoutWorldContext, DevelopmentOnly, CompactNodeTitle = "Should Not Happen"))
	static void ShouldNotHappen(const UObject* WorldContext);
	// Debug function that will print "Something has been done."
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Helpers", meta = (WorldContext = "WorldContext", CallableWithoutWorldContext, DevelopmentOnly, CompactNodeTitle = "Do Something"))
	static void DoSomething(const UObject* WorldContext);
	// Returns true if the game is running on the editor.
	UFUNCTION(BlueprintPure, Category = "Sweet Dreams|Core|Helpers")
	static bool IsRunningInEditor();
	// Returns the NOT value of the "IsRunningInEditor()" function.
	UFUNCTION(BlueprintPure, Category = "Sweet Dreams|Core|Helpers")
	static bool IsRunningInStandaloneGame();

	static inline UWorld* GetValidWorld(const UObject* WorldContext)
	{
		if (!IsValid(WorldContext)) return nullptr;
		return GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::ReturnNull);
	}
};
