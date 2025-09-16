// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/SweetDreamsCore.h"
#include "Save/SweetDreamsSaveInterface.h"
#include "Save/SweetDreamsSaveFile.h"
#include "Game/SweetDreamsGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/StreamableManager.h"
#include "Core/SweetDreamsSettings.h"
#include "Core/SweetDreamsBPLibrary.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "Engine/AssetManager.h"

USweetDreamsCore::USweetDreamsCore() {}

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
		SaveSlotPersistent = "SweetDreams_PERSISTENT";
	}
	if (SaveSlotLocal == "")
	{
		SaveSlotLocal = "SweetDreams_LOCAL";
	}
	// GET OTHER SUBSYSTEMS
}

void USweetDreamsCore::Initialize(FSubsystemCollectionBase& Collection)
{
	LoadSettings();
	PrintDream(nullptr, "Initializing Sweet Dreams Core subsystem.");
	if (CoreSettings->bEnableAutoCreateSave)
	{
		bool bSuccess = false;
		CreateSave(SaveClassPersistent, SaveSlotPersistent, bSuccess);
		CreateSave(SaveClassLocal, SaveSlotLocal, bSuccess);
	}
	Super::Initialize(Collection);
}

void USweetDreamsCore::Deinitialize()
{
	DeleteSave(SaveSlotLocal);
	PrintDream(nullptr, "Deinitializing Sweet Dreams Core subsystem.");
	Super::Deinitialize();
}

// Debug
void USweetDreamsCore::PrintDream(const UObject* DreamOrigin, FString Dream, EPrintType Severity, float Duration)
{
	if (!CoreSettings || !(CoreSettings->DebugFlags & static_cast<uint8>(EDebugFlags::PrintEnabled)))
	{
		return;
	}
	FString Origin = TEXT("[SweetDreams]");
	if (IsValid(DreamOrigin))
	{
		Origin = FString::Printf(TEXT("[%s]"), *DreamOrigin->GetName());
	}
	Dream = Origin + " " + Dream;
	FColor DreamColor;
	switch (Severity)
	{
	case EPrintType::INFO:
		DreamColor = CoreSettings->InfoColor;
		UE_LOG(LogCore, Display, TEXT("%s"), *Dream);
		break;
	case EPrintType::WARNING:
		DreamColor = CoreSettings->WarningColor;
		UE_LOG(LogCore, Warning, TEXT("%s"), *Dream);
		break;
	case EPrintType::ERROR:
		DreamColor = CoreSettings->ErrorColor;
		UE_LOG(LogCore, Error, TEXT("%s"), *Dream);
		break;
	default:
		DreamColor = FColor::White;
		UE_LOG(LogCore, Display, TEXT("%s"), *Dream);
		break;
	}
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, Duration, DreamColor, Dream);
	}
}

// SAVE

USweetDreamsSaveFile* USweetDreamsCore::CreateSave(TSubclassOf<USweetDreamsSaveFile> SaveClass, const FString& Slot, bool& bSuccess, int32 UserIndex)
{
	bSuccess = false;
	if (UGameplayStatics::DoesSaveGameExist(Slot, UserIndex))
	{
		if (CoreSettings->DebugFlags & static_cast<uint8>(EDebugFlags::PrintSaveOperations))
		{
			PrintDream(nullptr, FString::Printf(TEXT("%s EXISTS and WILL NOT be created. Returning NULL"), *Slot), EPrintType::WARNING);
		}
		return nullptr;
	}
	USaveGame* SaveObj = UGameplayStatics::CreateSaveGameObject(SaveClass);
	if (IsValid(SaveObj))
	{
		if (CoreSettings->DebugFlags & static_cast<uint8>(EDebugFlags::PrintSaveOperations))
		{
			PrintDream(nullptr, FString::Printf(TEXT("%s CREATED with SUCCESS."), *Slot));
		}
		bSuccess = true;
		USweetDreamsSaveFile* SweetSave = Cast<USweetDreamsSaveFile>(SaveObj);
		UpdateSaveReference(SweetSave, Slot);
		Save(Slot);
		return SweetSave;
	}
	if (CoreSettings->DebugFlags & static_cast<uint8>(EDebugFlags::PrintSaveOperations))
	{
		PrintDream(nullptr, FString::Printf(TEXT("%s NOT CREATED. Returning NULL."), *Slot), EPrintType::ERROR);
	}
	return nullptr;
}

bool USweetDreamsCore::Save(const FString& Slot, int32 UserIndex)
{
	USweetDreamsSaveFile* Save = GetSaveObject(Slot);
	if (!IsValid(Save)) return false;
	SaveData(Save);
	if (UGameplayStatics::SaveGameToSlot(Save, Slot, UserIndex))
	{
		if (CoreSettings->DebugFlags & static_cast<uint8>(EDebugFlags::PrintSaveOperations))
		{
			PrintDream(nullptr, FString::Printf(TEXT("%s SAVED with SUCCESS."), *Slot));
		}
		TArray<AActor*> Actors;
		UGameplayStatics::GetAllActorsWithInterface(GetWorld(), USweetDreamsSaveInterface::StaticClass(), Actors);
		for (AActor* Actor : Actors)
		{
			if (!IsValid(Actor)) continue;
			ISweetDreamsSaveInterface::Execute_OnGameSaved(Actor, Save, Slot);
		}
		return true;
	}
	if (CoreSettings->DebugFlags & static_cast<uint8>(EDebugFlags::PrintSaveOperations))
	{
		PrintDream(nullptr, FString::Printf(TEXT("%s FAILED to SAVE."), *Slot), EPrintType::ERROR);
	}
	return false;
}

USweetDreamsSaveFile* USweetDreamsCore::LoadSave(const FString& Slot, int32 UserIndex)
{
	if (USaveGame* SaveObject = UGameplayStatics::LoadGameFromSlot(Slot, UserIndex))
	{
		if (CoreSettings->DebugFlags & static_cast<uint8>(EDebugFlags::PrintSaveOperations))
		{
			PrintDream(nullptr, FString::Printf(TEXT("%s LOADED and returned with SUCCESS."), *Slot));
		}
		USweetDreamsSaveFile* SweetSave = Cast<USweetDreamsSaveFile>(SaveObject);
		UpdateSaveReference(SweetSave, Slot);
		LoadData(SweetSave);
		TArray<AActor*> Actors;
		UGameplayStatics::GetAllActorsWithInterface(GetWorld(), USweetDreamsSaveInterface::StaticClass(), Actors);
		for (AActor* Actor : Actors)
		{
			if (!IsValid(Actor)) continue;
			ISweetDreamsSaveInterface::Execute_OnGameSaved(Actor, SweetSave, Slot);
		}
		return SweetSave;
	}
	if (CoreSettings->DebugFlags & static_cast<uint8>(EDebugFlags::PrintSaveOperations))
	{
		PrintDream(nullptr, FString::Printf(TEXT("%s FAILED to LOAD."), *Slot), EPrintType::ERROR);
	}
	return nullptr;
}

bool USweetDreamsCore::DeleteSave(const FString& Slot, int32 UserIndex)
{
	if (UGameplayStatics::DeleteGameInSlot(Slot, UserIndex))
	{
		if (CoreSettings->DebugFlags & static_cast<uint8>(EDebugFlags::PrintSaveOperations))
		{
			PrintDream(nullptr, FString::Printf(TEXT("%s DELETED."), *Slot));
		}
		UpdateSaveReference(nullptr, Slot);
		return true;
	}
	if (CoreSettings->DebugFlags & static_cast<uint8>(EDebugFlags::PrintSaveOperations))
	{
		PrintDream(nullptr, FString::Printf(TEXT("%s FAILED to be DELETED."), *Slot), EPrintType::ERROR);
	}
	return false;
}

void USweetDreamsCore::UpdateSaveReference(USweetDreamsSaveFile* Save, FString Slot)
{
	if (Slot == SaveSlotPersistent)
	{
		SavePersistentRef = Save;
	}
	else if (Slot == SaveSlotLocal)
	{
		SaveLocalRef = Save;
	}
	else
	{
		CustomSaveFiles.FindOrAdd(Slot) = Save;
	}
}

FString USweetDreamsCore::GetCoreSaveSlot(bool bIsPersistent) const
{
	return bIsPersistent ? SaveSlotPersistent : SaveSlotLocal;
}

USweetDreamsSaveFile* USweetDreamsCore::GetSaveObject(const FString& Slot) const
{
	if (Slot == SaveSlotPersistent)
	{
		return SavePersistentRef;
	}
	else if (Slot == SaveSlotLocal)
	{
		return SaveLocalRef;
	}
	else
	{
		return *CustomSaveFiles.Find(Slot);
	}
}

void USweetDreamsCore::SaveData(USweetDreamsSaveFile* Save)
{
	if (!IsValid(Save)) return;
	FName CurrentLevelName = FName(GetWorld()->GetMapName());
	Save->SavedData.RemoveAll([&CurrentLevelName](const FSaveData& Data) {
		return Data.LevelName == CurrentLevelName;
		});
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsWithInterface(GetWorld(), USweetDreamsSaveInterface::StaticClass(), Actors);
	for (AActor* Actor : Actors)
	{
		if (!IsValid(Actor)) continue;
		FSaveData Data;
		Data.ActorName = Actor->GetFName();
		Data.LevelName = CurrentLevelName;
		Data.CustomData = ISweetDreamsSaveInterface::Execute_GetCustomData(Actor);
		FMemoryWriter MemoryWriter(Data.ByteData);
		FObjectAndNameAsStringProxyArchive Archive(MemoryWriter, true);
		Archive.ArIsSaveGame = true;
		Actor->Serialize(Archive);
		Save->SavedData.Add(Data);
	}
	Save->LastTimeSaved = FDateTime::Now();
	Save->AmountOfSaves++;
}

void USweetDreamsCore::LoadCoreSaves()
{
	LoadSave(SaveSlotPersistent);
	LoadSave(SaveSlotLocal);
}

void USweetDreamsCore::LoadData(USweetDreamsSaveFile* Save)
{
	if (!IsValid(Save)) return;
	FName CurrentLevelName = FName(GetWorld()->GetMapName());
	for (const FSaveData& Data : Save->SavedData)
	{
		AActor* Actor = FindActorByName(Data.ActorName);
		if (!IsValid(Actor)) continue;
		FMemoryReader MemoryReader(Data.ByteData);
		FObjectAndNameAsStringProxyArchive Archive(MemoryReader, true);
		Archive.ArIsSaveGame = true;
		Actor->Serialize(Archive);
		ISweetDreamsSaveInterface::Execute_LoadCustomData(Actor, Data.CustomData);
	}
}

AActor* USweetDreamsCore::FindActorByName(FName Name)
{
	if (!GetWorld()) return nullptr;
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
