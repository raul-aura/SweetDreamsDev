// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SweetDreamsCore.generated.h"

class USweetDreamsSaveFile;
class USweetDreamsSettings;

UENUM(BlueprintType)
enum class EPrintType : uint8
{
	INFO,
	WARNING,
	ERROR
};

UCLASS(Category = "SweetDreams|Core")
class SWEETDREAMS_API USweetDreamsCore : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	USweetDreamsCore();
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	void LoadSettings();
	const USweetDreamsSettings* CoreSettings;

	// DEBUG
	void PrintDream(const UObject* DreamOrigin, FString Dream, EPrintType Severity = EPrintType::INFO, float duration = 4.0f);

	// SAVE
	USweetDreamsSaveFile* CreateSave(TSubclassOf<USweetDreamsSaveFile> SaveClass, const FString& Slot, bool& bSuccess, int32 UserIndex = 0);
	bool Save(const FString& Slot, int32 UserIndex = 0);
	TObjectPtr<USweetDreamsSaveFile> LoadSave(const FString& Slot, int32 UserIndex = 0);
	bool DeleteSave(const FString& Slot, int32 UserIndex = 0);
	//
	void SaveData(TObjectPtr<USweetDreamsSaveFile> Save);
	void LoadCoreSaves();
	void LoadData(TObjectPtr<USweetDreamsSaveFile> Save);
	//
	void UpdateSaveReference(TObjectPtr<USweetDreamsSaveFile> Save, FString Slot);
	FString GetCoreSaveSlot(bool bIsPersistent = true) const;
	TObjectPtr<USweetDreamsSaveFile> GetSaveObject(const FString& Slot) const;

	// WORLD
	TObjectPtr<AActor> FindActorByName(FName Name);
	TArray<TObjectPtr<AActor>> GetAllActorsWorld() const;

	// LOADING
	void LoadLevel(TSoftObjectPtr<UWorld> Level);
	TSoftObjectPtr<UWorld> CurrentLoadingLevel;
	bool bIsLoadingLevel = false;

	// GLOBAL BOOLs + INTs32
	void SetGlobalInt(FName Key, int32 Value);
	int32 GetGlobalInt(FName Key);
	void SetGlobalBool(FName Key, bool Value);
	bool GetGlobalBool(FName Key);
	TMap<FName, int32> GlobalInts;
	TMap<FName, bool> GlobalBools;

protected:

	// SAVE
	TMap<FString, TObjectPtr<USweetDreamsSaveFile>> CustomSaveFiles;
	TObjectPtr<USweetDreamsSaveFile> SavePersistentRef = nullptr;
	TObjectPtr<USweetDreamsSaveFile> SaveLocalRef = nullptr;
	//
	TSubclassOf<USweetDreamsSaveFile> SaveClassPersistent = nullptr;
	TSubclassOf<USweetDreamsSaveFile> SaveClassLocal = nullptr;
	//
	FString SaveSlotPersistent = "SweetDream_PERSISTENT";
	FString SaveSlotLocal = "SweetDream_LOCAL";
};
