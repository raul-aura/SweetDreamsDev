// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SweetDreamsCore.generated.h"

class USweetDreamsSaveFile;
class USweetDreamsSettings;
namespace ELogVerbosity { enum Type : uint8; }

DECLARE_LOG_CATEGORY_EXTERN(LogSweetDreams, Log, All);

using FNativeLogFn = TFunction<void(ELogVerbosity::Type, const TCHAR*)>;

UENUM(BlueprintType)
enum class EPrintType : uint8
{
	Info,
	Warning,
	Error
};

UCLASS(Category = "SweetDreams|Core")
class SWEETDREAMS_API USweetDreamsCore : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	/*
	Initialization of settings and the save system.
	*/
	USweetDreamsCore();
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	void LoadSettings();
	const USweetDreamsSettings* CoreSettings;

	/*
	Debugging functions.
	The second Log function receives a FNativeLogFn log function so that other subsystems or classes can override the logging category.
	Log_Internal is a helper function, best suited for this class, to log only to the console and log file, receiving just a text and severity.
	*/
	void Log(const UObject* WorldContext, const FString& Text, EPrintType Severity = EPrintType::Info, float Duration = 4.0f, bool bLogToScreen = true);
	void Log(const UObject* WorldContext, const FString& Text, FNativeLogFn LogFunction, EPrintType Severity = EPrintType::Info, float Duration = 4.0f, bool bLogToScreen = true);
	void Log_Internal(FString Text, EPrintType Severity = EPrintType::Info);

	/*
	Save operations to creates a save files, save a file to disk, delete a file, load a file from disk and helper functions.
	The save system uses the USweetDreamsSaveFile class for the save file object, and uses the USweetDreamsSaveInterface interface
	to filter actors that are willing to react with the system.
	By default, the save system will automatically serialize and save to disk variables marked with the "Save Game" flag.
	*/
	USweetDreamsSaveFile* CreateSave(TSubclassOf<USweetDreamsSaveFile> SaveClass, const FString& Slot, bool& bSuccess, int32 UserIndex = 0);
	bool Save(const FString& Slot, int32 UserIndex = 0);
	TObjectPtr<USweetDreamsSaveFile> LoadSave(const FString& Slot, int32 UserIndex = 0);
	bool DeleteSave(const FString& Slot, int32 UserIndex = 0);
	void SaveData(TObjectPtr<USweetDreamsSaveFile> Save);
	void LoadCoreSaves();
	void LoadData(TObjectPtr<USweetDreamsSaveFile> Save);
	void UpdateSaveReference(TObjectPtr<USweetDreamsSaveFile> Save, FString Slot);
	FString GetCoreSaveSlot(bool bIsPersistent = true) const;
	TObjectPtr<USweetDreamsSaveFile> GetSaveObject(const FString& Slot) const;
	/*
	Helper functions for the save system to get actors.
	*/
	TObjectPtr<AActor> FindActorByName(FName Name);
	TArray<TObjectPtr<AActor>> GetAllActorsWorld() const;

	/*
	Level loading system that communicates with the ASweetDreamsGameMode class.
	*/
	void LoadLevel(TSoftObjectPtr<UWorld> Level);
	TSoftObjectPtr<UWorld> CurrentLoadingLevel;
	bool bIsLoadingLevel = false;

	/*
	Functions for the global variable (ints and bools) library.
	This is similar to the RPG Maker's system of Variables and Switches, which is a global library of numbers and booleans.
	*/
	void SetGlobalInt(FName Key, int32 Value);
	int32 GetGlobalInt(FName Key);
	void SetGlobalBool(FName Key, bool Value);
	bool GetGlobalBool(FName Key);
	TMap<FName, int32> GlobalInts;
	TMap<FName, bool> GlobalBools;

protected:

	/*
	Variables for the save system.
	*/
	TMap<FString, TObjectPtr<USweetDreamsSaveFile>> CustomSaveFiles;
	TObjectPtr<USweetDreamsSaveFile> SavePersistentRef = nullptr;
	TObjectPtr<USweetDreamsSaveFile> SaveLocalRef = nullptr;
	TSubclassOf<USweetDreamsSaveFile> SaveClassPersistent = nullptr;
	TSubclassOf<USweetDreamsSaveFile> SaveClassLocal = nullptr;
	FString SaveSlotPersistent = "SweetDream_PERSISTENT";
	FString SaveSlotLocal = "SweetDream_LOCAL";
};
