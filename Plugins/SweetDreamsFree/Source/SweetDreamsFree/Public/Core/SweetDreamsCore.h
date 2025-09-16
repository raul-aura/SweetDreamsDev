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

	USweetDreamsCore();
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	void LoadSettings();
	const USweetDreamsSettings* CoreSettings;

	// DEBUG
	void PrintDream(const UObject* DreamOrigin, FString Dream, EPrintType Severity = EPrintType::INFO, float duration = 4.0f);

	// SAVE
	USweetDreamsSaveFile* CreateSave(TSubclassOf<USweetDreamsSaveFile> SaveClass, const FString& Slot, bool& bSuccess, int32 UserIndex = 0);
	bool Save(const FString& Slot, int32 UserIndex = 0);
	USweetDreamsSaveFile* LoadSave(const FString& Slot, int32 UserIndex = 0);
	bool DeleteSave(const FString& Slot, int32 UserIndex = 0);
	//
	void SaveData(USweetDreamsSaveFile* Save);
	void LoadCoreSaves();
	void LoadData(USweetDreamsSaveFile* Save);
	//
	void UpdateSaveReference(USweetDreamsSaveFile* Save, FString Slot);
	FString GetCoreSaveSlot(bool bIsPersistent = true) const;
	USweetDreamsSaveFile* GetSaveObject(const FString& Slot) const;

	// WORLD
	AActor* FindActorByName(FName Name);

	// LOADING
	void LoadLevel(TSoftObjectPtr<UWorld> Level);
	TSoftObjectPtr<UWorld> CurrentLoadingLevel;

protected:


	// SAVE
	TMap<FString, USweetDreamsSaveFile*> CustomSaveFiles;
	USweetDreamsSaveFile* SavePersistentRef = nullptr;
	USweetDreamsSaveFile* SaveLocalRef = nullptr;
	//
	TSubclassOf<USweetDreamsSaveFile> SaveClassPersistent = nullptr;
	TSubclassOf<USweetDreamsSaveFile> SaveClassLocal = nullptr;
	//
	FString SaveSlotPersistent = "SweetDream_PERSISTENT";
	FString SaveSlotLocal = "SweetDream_LOCAL";
};
