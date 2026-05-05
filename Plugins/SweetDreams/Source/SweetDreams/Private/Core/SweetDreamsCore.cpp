// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/SweetDreamsCore.h"
#include "Save/SweetDreamsSaveInterface.h"
#include "Save/SweetDreamsSaveFile.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/StreamableManager.h"
#include "Core/SweetDreamsSettings.h"
#include "Core/SweetDreamsBPLibrary.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "Engine/AssetManager.h"
#include "EngineUtils.h"

DEFINE_LOG_CATEGORY(LogSweetDreams);

USweetDreamsCore::USweetDreamsCore() {}

void USweetDreamsCore::LoadSettings()
{
	CoreSettings = GetMutableDefault<USweetDreamsSettings>();

	SaveSlotPersistent = CoreSettings->PersistentSlot;
	SaveClassPersistent = CoreSettings->PersistentClass;

	if (SaveSlotPersistent == "")
	{
		SaveSlotPersistent = "SweetDreams_PERSISTENT";
	}
}

void USweetDreamsCore::Initialize(FSubsystemCollectionBase& Collection)
{
	LoadSettings();
	Log_Internal("Initializing Sweet Dreams Core subsystem.");

	if (CoreSettings->bEnableAutoCreateSave)
	{
		bool bSuccess = false;
		CreateSave(SaveClassPersistent, SaveSlotPersistent, bSuccess);
	}

	Super::Initialize(Collection);
}

void USweetDreamsCore::Deinitialize()
{
	Log_Internal("Deinitializing Sweet Dreams Core subsystem.");

	Super::Deinitialize();
}

static FNativeLogFn DefaultLog()
{
	return [](ELogVerbosity::Type Verbosity, const TCHAR* Text)
	{
		switch (Verbosity)
		{
		case ELogVerbosity::Error:
			UE_LOG(LogSweetDreams, Error, TEXT("%s"), Text);
			break;
		case ELogVerbosity::Warning:
			UE_LOG(LogSweetDreams, Warning, TEXT("%s"), Text);
			break;
		default:
			UE_LOG(LogSweetDreams, Display, TEXT("%s"), Text);
			break;
		}
	};
}

void USweetDreamsCore::Log(const UObject* WorldContext, const FString& Text, EPrintType Severity, float Duration, bool bLogToScreen)
{
	Log(WorldContext, Text, DefaultLog(), Severity, Duration, bLogToScreen);
}

void USweetDreamsCore::Log(const UObject* WorldContext, const FString& Text, FNativeLogFn LogFunction, EPrintType Severity, float Duration, bool bLogToScreen)
{
	if (!CoreSettings || !CoreSettings->bLogEnabled) return;

	const FString Origin = IsValid(WorldContext)
		? FString::Printf(TEXT("[%s]"), *WorldContext->GetName())
		: TEXT("[SweetDreams]");

	const FString LogText = Origin + TEXT(" ") + Text;
	FColor Color = CoreSettings->InfoColor;
	ELogVerbosity::Type Verbosity = ELogVerbosity::Display;

	switch (Severity)
	{
	case EPrintType::Warning:
		Color = CoreSettings->WarningColor;
		Verbosity = ELogVerbosity::Warning;
		break;

	case EPrintType::Error:
		Color = CoreSettings->ErrorColor;
		Verbosity = ELogVerbosity::Error;
		break;

	case EPrintType::Info:
	default:
		break;
	}

	if (!LogFunction)
	{
		LogFunction = DefaultLog();
	}
	LogFunction(Verbosity, *LogText);

	if (GEngine && bLogToScreen)
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, Duration, Color, LogText);
	}
}

void USweetDreamsCore::Log_Internal(FString Text, EPrintType Severity)
{
	Log(nullptr, Text, Severity, 0.f, false);
}

USweetDreamsSaveFile* USweetDreamsCore::CreateSave(TSubclassOf<USweetDreamsSaveFile> SaveClass, const FString& Slot, bool& bSuccess, int32 UserIndex)
{
	bSuccess = false;

	if (UGameplayStatics::DoesSaveGameExist(Slot, UserIndex))
	{
		if (CoreSettings && CoreSettings->bSaveOperations)
		{
			Log_Internal(FString::Printf(TEXT("%s EXISTS and WILL NOT be created. Returning NULL"), *Slot), EPrintType::Warning);
		}
		return nullptr;
	}

	USaveGame* SaveObj = UGameplayStatics::CreateSaveGameObject(SaveClass);
	if (IsValid(SaveObj))
	{
		if (CoreSettings && CoreSettings->bSaveOperations)
		{
			Log_Internal(FString::Printf(TEXT("%s CREATED with SUCCESS."), *Slot));
		}

		bSuccess = true;
		USweetDreamsSaveFile* SweetSave = Cast<USweetDreamsSaveFile>(SaveObj);
		UpdateSaveReference(SweetSave, Slot);
		Save(Slot);

		return SweetSave;
	}

	if (CoreSettings && CoreSettings->bSaveOperations)
	{
		Log_Internal(FString::Printf(TEXT("%s NOT CREATED. Returning NULL."), *Slot), EPrintType::Error);
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
		if (CoreSettings && CoreSettings->bSaveOperations)
		{
			Log_Internal(FString::Printf(TEXT("%s SAVED with SUCCESS."), *Slot));
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

	if (CoreSettings && CoreSettings->bSaveOperations)
	{
		Log_Internal(FString::Printf(TEXT("%s FAILED to SAVE."), *Slot), EPrintType::Error);
	}

	return false;
}

TObjectPtr<USweetDreamsSaveFile> USweetDreamsCore::LoadSave(const FString& Slot, int32 UserIndex)
{
	if (USaveGame* SaveObject = UGameplayStatics::LoadGameFromSlot(Slot, UserIndex))
	{
		if (CoreSettings && CoreSettings->bSaveOperations)
		{
			Log_Internal(FString::Printf(TEXT("%s LOADED and returned with SUCCESS."), *Slot));
		}

		USweetDreamsSaveFile* SweetSave = Cast<USweetDreamsSaveFile>(SaveObject);
		UpdateSaveReference(SweetSave, Slot);
		LoadData(SweetSave);

		TArray<AActor*> Actors;
		UGameplayStatics::GetAllActorsWithInterface(GetWorld(), USweetDreamsSaveInterface::StaticClass(), Actors);

		for (AActor* Actor : Actors)
		{
			if (!IsValid(Actor)) continue;
			ISweetDreamsSaveInterface::Execute_OnGameLoaded(Actor, SweetSave, Slot);
		}

		return SweetSave;
	}

	if (CoreSettings && CoreSettings->bSaveOperations)
	{
		Log_Internal(FString::Printf(TEXT("%s FAILED to LOAD."), *Slot), EPrintType::Error);
	}

	return nullptr;
}

bool USweetDreamsCore::DeleteSave(const FString& Slot, int32 UserIndex)
{
	if (UGameplayStatics::DeleteGameInSlot(Slot, UserIndex))
	{
		if (CoreSettings && CoreSettings->bSaveOperations)
		{
			Log_Internal(FString::Printf(TEXT("%s DELETED."), *Slot));
		}

		UpdateSaveReference(nullptr, Slot);

		return true;
	}

	if (CoreSettings && CoreSettings->bSaveOperations)
	{
		Log_Internal(FString::Printf(TEXT("%s FAILED to be DELETED."), *Slot), EPrintType::Error);
	}

	return false;
}

void USweetDreamsCore::UpdateSaveReference(TObjectPtr<USweetDreamsSaveFile> Save, FString Slot)
{
	if (Slot == SaveSlotPersistent)
	{
		SavePersistentRef = Save;
	}
	else
	{
		CustomSaveFiles.FindOrAdd(Slot) = Save;
	}
}

FString USweetDreamsCore::GetCoreSaveSlot() const
{
	return SaveSlotPersistent;
}

TObjectPtr<USweetDreamsSaveFile> USweetDreamsCore::GetSaveObject(const FString& Slot) const
{
	if (Slot == SaveSlotPersistent)
	{
		return SavePersistentRef;
	}
	else
	{
		return *CustomSaveFiles.Find(Slot);
	}
}

void USweetDreamsCore::SaveData(TObjectPtr<USweetDreamsSaveFile> Save)
{
	if (!IsValid(Save)) return;

	FName CurrentLevelName = FName(GetWorld()->GetMapName());

	Save->SavedData.RemoveAll([&CurrentLevelName](const FSaveData& Data)
	{
		return Data.LevelName == CurrentLevelName;
	});

	for (AActor* Actor : TActorRange<AActor>(GetWorld()))
	{
		if (!IsValid(Actor) || !Actor->Implements<USweetDreamsSaveInterface>()) continue;

		FSaveData Data;
		Data.ActorName = Actor->GetFName();
		Data.LevelName = CurrentLevelName;
		Data.CustomData = ISweetDreamsSaveInterface::Execute_GetCustomData(Actor);

		FMemoryWriter MemoryWriter(Data.ByteData, true);
		FObjectAndNameAsStringProxyArchive Archive(MemoryWriter, false);
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
}

void USweetDreamsCore::LoadData(TObjectPtr<USweetDreamsSaveFile> Save)
{
	if (!IsValid(Save)) return;

	FName CurrentLevelName = FName(GetWorld()->GetMapName());

	for (const FSaveData& Data : Save->SavedData)
	{
		AActor* Actor = FindActorByName(Data.ActorName);
		if (!IsValid(Actor) || !Actor->Implements<USweetDreamsSaveInterface>()) continue;

		FMemoryReader MemoryReader(Data.ByteData);
		FObjectAndNameAsStringProxyArchive Archive(MemoryReader, true);
		Archive.ArIsSaveGame = true;
		Actor->Serialize(Archive);

		ISweetDreamsSaveInterface::Execute_LoadCustomData(Actor, Data.CustomData);
	}
}

TObjectPtr<AActor> USweetDreamsCore::FindActorByName(FName Name)
{
	if (!GetWorld()) return nullptr;

	for (AActor* Actor : TActorRange<AActor>(GetWorld()))
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

	bIsLoadingLevel = true;
	CurrentLoadingLevel = Level;
	OnLevelLoadStarted.Broadcast(Level.Get());

	TArray<FSoftObjectPath> AssetList;
	AssetList.Add(Level.ToSoftObjectPath());
	FStreamableManager& StreamableManager = UAssetManager::GetStreamableManager();
	StreamableManager.RequestAsyncLoad(
		AssetList, 
		FStreamableDelegate::CreateLambda([this, Level]()
		{
			OnLevelLoadFinished.Broadcast(Level.Get());
		})
	);
}

void USweetDreamsCore::SetGlobalInt(FName Key, int32 Value)
{
	GlobalInts.Add(Key, Value);

	if (CoreSettings && CoreSettings->bSettingGlobalVariable)
	{
		Log_Internal(FString::Printf(TEXT("Setting %s with value: %d"), *Key.ToString(), Value));
	}
}

int32 USweetDreamsCore::GetGlobalInt(FName Key)
{
	if (int32* Value = GlobalInts.Find(Key))
	{
		return *Value;
	}

	return INDEX_NONE;
}

void USweetDreamsCore::SetGlobalBool(FName Key, bool Value)
{
	GlobalBools.Add(Key, Value);
	
	if (CoreSettings && CoreSettings->bSettingGlobalVariable)
	{
		Log_Internal(FString::Printf(TEXT("Setting %s with value: %s"), *Key.ToString(), Value ? TEXT("true") : TEXT("false")));
	}
}

bool USweetDreamsCore::GetGlobalBool(FName Key)
{
	if (bool* Value = GlobalBools.Find(Key))
	{
		return *Value;
	}

	return false;
}
