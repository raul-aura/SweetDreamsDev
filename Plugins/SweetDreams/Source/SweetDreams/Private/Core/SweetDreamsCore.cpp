// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/SweetDreamsCore.h"
#include "Save/SweetDreamsSaveInterface.h"
#include "Game/SweetDreamsGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/StreamableManager.h"
#include "Core/SweetDreamsSettings.h"
#include "Save/SaveData.h"
#include "Core/SweetDreamsBPLibrary.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "Engine/AssetManager.h"

USweetDreamsCore::USweetDreamsCore()
	:
	UserSettings(FDreamUserSettings())
{

}

void USweetDreamsCore::LoadSettings()
{
	CoreSettings = GetMutableDefault<USweetDreamsSettings>();

	// SAVE
	SaveSlotPersistent = CoreSettings->PersistentSlot;
	SaveSlotLocal = CoreSettings->LocalSlot;
	SaveClassPersistent = CoreSettings->PersistentClass;
	SaveClassLocal = CoreSettings->LocalClass;
	if (SaveSlotPersistent == "")
	{
		SaveSlotPersistent = "SweetDream_PERSISTENT";
	}
	if (SaveSlotLocal == "")
	{
		SaveSlotLocal = "SweetDream_LOCAL";
	}

	// GET OTHER SUBSYSTEMS
}

void USweetDreamsCore::Initialize(FSubsystemCollectionBase& Collection)
{
	LoadSettings();
	PrintDream(nullptr, "Initializing Sweet Dreams Core subsystem.");
	if (CoreSettings->bEnableAutoCreateSave)
	{
		CreateSave(SaveClassPersistent);
		CreateSave(SaveClassLocal, false);
	}
	if (CoreSettings->bEnableAutoLoadSave)
	{
		LoadSave();
		LoadSave(false);
	}
	Super::Initialize(Collection);
}

void USweetDreamsCore::Deinitialize()
{
	DeleteSave(false);
	Super::Deinitialize();
}

// Debug
void USweetDreamsCore::PrintDream(const UObject* DreamOrigin, FString Dream, EPrintType Severity, float Duration)
{
	if (!(CoreSettings->DebugFlags & static_cast<uint8>(EDebugFlags::PrintEnabled))) return;
	FString Origin = "[SweetDreams]";
	if (DreamOrigin)
	{
		Origin = FString::Printf(TEXT("[%s]"), *DreamOrigin->GetName());
	}
	Dream = Origin + " " + Dream;
	FColor DreamColor;
	switch (Severity)
	{
	case EPrintType::INFO:
		DreamColor = FColor(195, 150, 255);
		break;
	case EPrintType::WARNING:
		DreamColor = FColor(255, 191, 64);
		break;
	case EPrintType::ERROR:
		DreamColor = FColor(216, 29, 29);
		break;
	}
	UE_LOG(LogCore, Display, TEXT("%s"), *Dream);
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, Duration, DreamColor, Dream);
	}
}

// Settings
void USweetDreamsCore::SetUserSettings(FDreamUserSettings Settings)
{
	PrintDream(nullptr, "Overriding User Settings.");
	UserSettings = Settings;
}

FDreamUserSettings USweetDreamsCore::GetUserSettings() const
{
	return UserSettings;
}

// SAVE

bool USweetDreamsCore::CreateSave(TSubclassOf<USweetDreamsSaveFile> SaveClass, bool bIsPersistent)
{
	FString SaveSlot = bIsPersistent ? SaveSlotPersistent : SaveSlotLocal;
	FString SaveName = bIsPersistent ? "Persistent Save" : "Local Save";
	if (UGameplayStatics::DoesSaveGameExist(SaveSlot, 0))
	{
		if (CoreSettings->DebugFlags & static_cast<uint8>(EDebugFlags::PrintSaveOperations))
		{
			PrintDream(nullptr, FString::Printf(TEXT("%s ALREADY EXISTS and WILL NOT be created again."), *SaveName));
		}
		return false;
	}
	USaveGame* SaveObject = UGameplayStatics::CreateSaveGameObject(SaveClass);
	if (SaveObject)
	{
		if (CoreSettings->DebugFlags & static_cast<uint8>(EDebugFlags::PrintSaveOperations))
		{
			PrintDream(nullptr, FString::Printf(TEXT("%s CREATED with SUCCESS."), *SaveName));
		}
		if (bIsPersistent)
		{
			SavePersistentRef = Cast<USweetDreamsSaveFile>(SaveObject);
			Save(SavePersistentRef);
		}
		else
		{
			SaveLocalRef = Cast<USweetDreamsSaveFile>(SaveObject);
			Save(SaveLocalRef, false);
		}
		return true;
	}
	if (CoreSettings->DebugFlags & static_cast<uint8>(EDebugFlags::PrintSaveOperations))
	{
		PrintDream(nullptr, FString::Printf(TEXT("%s NOT CREATED. %s reference will be NULL."), *SaveName));
	}
	return false;
}

bool USweetDreamsCore::Save(USweetDreamsSaveFile* SaveObject, bool bIsPersistent, bool bSaveAllData)
{
	FString SaveSlot = bIsPersistent ? SaveSlotPersistent : SaveSlotLocal;
	FString SaveName = bIsPersistent ? "Persistent Save" : "Local Save";
	SaveData(bIsPersistent, bSaveAllData);
	if (UGameplayStatics::SaveGameToSlot(SaveObject, SaveSlot, 0))
	{
		if (CoreSettings->DebugFlags & static_cast<uint8>(EDebugFlags::PrintSaveOperations))
		{
			PrintDream(nullptr, FString::Printf(TEXT("%s SAVED with SUCCESS."), *SaveName));
		}
		return true;
	}
	if (CoreSettings->DebugFlags & static_cast<uint8>(EDebugFlags::PrintSaveOperations))
	{
		PrintDream(nullptr, FString::Printf(TEXT("%s FAILED to SAVE."), *SaveName));
	}
	return false;
}

USweetDreamsSaveFile* USweetDreamsCore::LoadSave(bool bIsPersistent, int32 Version)
{
	FString SaveSlot = bIsPersistent ? SaveSlotPersistent : SaveSlotLocal;
	FString SaveName = bIsPersistent ? "Persistent Save" : "Local Save";
	if (USaveGame* SaveObject = UGameplayStatics::LoadGameFromSlot(SaveSlot, 0))
	{
		if (CoreSettings->DebugFlags & static_cast<uint8>(EDebugFlags::PrintSaveOperations))
		{
			PrintDream(nullptr, FString::Printf(TEXT("%s LOADED and returned with SUCCESS."), *SaveName));
		}
		if (bIsPersistent)
		{
			SavePersistentRef = Cast<USweetDreamsSaveFile>(SaveObject);
			LoadData(true, Version);
			return SavePersistentRef;
		}
		else
		{
			SaveLocalRef = Cast<USweetDreamsSaveFile>(SaveObject);
			LoadData(false, Version);
			return SaveLocalRef;
		}
	}
	if (CoreSettings->DebugFlags & static_cast<uint8>(EDebugFlags::PrintSaveOperations))
	{
		PrintDream(nullptr, FString::Printf(TEXT("%s FAILED to LOAD."), *SaveName));
	}
	return nullptr;
}

void USweetDreamsCore::SaveData(bool bIsPersistent, bool bSaveAllData)
{
	USweetDreamsSaveFile* SaveObject = GetSaveObject(bIsPersistent);
	if (!IsValid(SaveObject)) return;
	SaveObject->SavedDeltaData.Empty();
	FDeltaSaveData NewDelta;
	NewDelta.SaveTime = FDateTime::Now();
	if (bSaveAllData)
	{
		SaveObject->SavedFullData.Empty();
		SaveObject->CurrentVersion = 0;
	}
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsWithInterface(GetWorld(), USweetDreamsSaveInterface::StaticClass(), Actors);
	for (AActor* Actor : Actors)
	{
		if (!IsValid(Actor)) continue;
		FSaveData Data;
		Data.ActorName = Actor->GetFName();
		if (bSaveAllData)
		{
			Data.CustomData = bIsPersistent ? 
				ISweetDreamsSaveInterface::Execute_GetPersistentCustomData(Actor) : 
				ISweetDreamsSaveInterface::Execute_GetLocalCustomData(Actor);
			FMemoryWriter MemoryWriter(Data.ByteData);
			FObjectAndNameAsStringProxyArchive Archive(MemoryWriter, true);
			Archive.ArIsSaveGame = true;
			Actor->Serialize(Archive);
			SaveObject->SavedFullData.Add(Data);
		}
		else
		{
			if (ISweetDreamsSaveInterface::Execute_IsDeltaSaveEnabled(Actor))
			{
				Data.CustomDeltaData = bIsPersistent ?
					ISweetDreamsSaveInterface::Execute_GetPersistentDeltaData(Actor) :
					ISweetDreamsSaveInterface::Execute_GetLocalDeltaData(Actor);
				NewDelta.DeltaData.Add(Data);
			}
		}
	}
	SaveObject->SavedDeltaData.Add(NewDelta);
	SaveObject->UpdateCurrentVersion();
}

void USweetDreamsCore::LoadData(bool bIsPersistent, int32 Version)
{
	USweetDreamsSaveFile* SaveObject = GetSaveObject(bIsPersistent);
	if (!IsValid(SaveObject)) return;
	Version = FMath::Clamp(Version, 0, SaveObject->CurrentVersion);
	for (const FSaveData& Data : SaveObject->SavedFullData)
	{
		AActor* Actor = FindActorByName(Data.ActorName);
		if (!IsValid(Actor)) continue;
		FMemoryReader MemoryReader(Data.ByteData);
		FObjectAndNameAsStringProxyArchive Archive(MemoryReader, true);
		Archive.ArIsSaveGame = true;
		Actor->Serialize(Archive);
		bIsPersistent ?
			ISweetDreamsSaveInterface::Execute_LoadPersistentCustomData(Actor, Data.CustomData) :
			ISweetDreamsSaveInterface::Execute_LoadLocalCustomData(Actor, Data.CustomData);
	}
	if (Version < 0) return;
	for (int32 i = 0; i <= Version; i++)
	{
		for (const FSaveData& DeltaData : SaveObject->SavedDeltaData[i].DeltaData)
		{
			AActor* Actor = FindActorByName(DeltaData.ActorName);
			if (!IsValid(Actor)) continue;
			bIsPersistent ?
				ISweetDreamsSaveInterface::Execute_LoadPersistentDeltaData(Actor, DeltaData.CustomDeltaData) :
				ISweetDreamsSaveInterface::Execute_LoadLocalDeltaData(Actor, DeltaData.CustomDeltaData);
		}
	}
}

bool USweetDreamsCore::DeleteSave(bool bIsPersistent)
{
	FString SaveSlot = bIsPersistent ? SaveSlotPersistent : SaveSlotLocal;
	return UGameplayStatics::DeleteGameInSlot(SaveSlot, 0);
}

USweetDreamsSaveFile* USweetDreamsCore::GetSaveObject(bool bIsPersistent) const
{
	USweetDreamsSaveFile* SaveObject;
	if (bIsPersistent)
	{
		SaveObject = SavePersistentRef;
	}
	else
	{
		SaveObject = SaveLocalRef;
	}
	return SaveObject;
}

AActor* USweetDreamsCore::FindActorByName(FName Name)
{
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsWithInterface(GetWorld(), USweetDreamsSaveInterface::StaticClass(), Actors);
	for (AActor* Actor : Actors)
	{
		if (IsValid(Actor) && Actor->GetFName().IsEqual(Name))
		{
			return Actor;
		}
	}
	return nullptr;
}

void USweetDreamsCore::LoadLevel(TSoftObjectPtr<UWorld> Level)
{
	if (Level.IsNull()) return;
	CurrentLoadingLevel = Level;
	TArray<FSoftObjectPath> AssetList;
	AssetList.Add(Level.ToSoftObjectPath());
	ASweetDreamsGameMode* DreamGameMode = Cast<ASweetDreamsGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (IsValid(DreamGameMode))
	{
		DreamGameMode->LevelLoadStarted(Level);
	}
	FStreamableManager& StreamableManager = UAssetManager::GetStreamableManager();
	FStreamableDelegate StreamableDelegate;
	StreamableDelegate.BindLambda([this, DreamGameMode, Level]() {
		if (IsValid(DreamGameMode))
		{
			DreamGameMode->LevelLoadFinished(Level);
		}
		});
	StreamableManager.RequestAsyncLoad(AssetList, StreamableDelegate);
}
