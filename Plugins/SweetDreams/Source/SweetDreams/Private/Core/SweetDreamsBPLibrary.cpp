// Copyright Epic Games, Inc. All Rights Reserved.

#include "Core/SweetDreamsBPLibrary.h"
#include "Core/SweetDreamsCore.h"
#include "Core/SweetDreamsSettings.h"
#include "Game/SweetDreamsGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Online.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "Engine/Engine.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerState.h"

USweetDreamsBPLibrary::USweetDreamsBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer) {}

USweetDreamsCore* USweetDreamsBPLibrary::GetSweetDreamsCore(const UObject* WorldContext)
{
	if (UWorld* World = GetValidWorld(WorldContext))
	{
		if (UGameInstance* GameInstance = World->GetGameInstance())
		{
			return GameInstance->GetSubsystem<USweetDreamsCore>();
		}
	}
	return nullptr;
}

ASweetDreamsGameMode* USweetDreamsBPLibrary::GetSweetDreamsGameMode(const UObject* WorldContext)
{
	if (UWorld* World = GetValidWorld(WorldContext))
	{
		return Cast<ASweetDreamsGameMode>(UGameplayStatics::GetGameMode(World));
	}
	return nullptr;
}

FString USweetDreamsBPLibrary::GetGameVersion(const UObject* WorldContext)
{
	if (USweetDreamsCore* Core = GetSweetDreamsCore(WorldContext))
	{
		return Core->CoreSettings->GameVersion;
	}
	return TEXT("No Sweet Dreams Core or Sweet Dreams Core Settings found.");
}

float USweetDreamsBPLibrary::GetFPS(const UObject* WorldContext)
{
	if (UWorld* World = GetValidWorld(WorldContext))
	{
		return 1.f / World->GetDeltaSeconds();
	}

	return 0.0f;
}

USweetDreamsSaveFile* USweetDreamsBPLibrary::CreateCustomSave(const UObject* WorldContext, TSubclassOf<USweetDreamsSaveFile> SaveClass, FString CustomSlot, bool& bSuccessful)
{
	if (USweetDreamsCore* Core = GetSweetDreamsCore(WorldContext))
	{
		return Core->CreateSave(SaveClass, CustomSlot, bSuccessful);
	}
	return nullptr;
}

USweetDreamsSaveFile* USweetDreamsBPLibrary::CreatePersistentSave(const UObject* WorldContext, TSubclassOf<USweetDreamsSaveFile> SaveClass, bool& bSuccessful)
{
	if (USweetDreamsCore* Core = GetSweetDreamsCore(WorldContext))
	{
		FString Slot = Core->GetCoreSaveSlot();
		return Core->CreateSave(SaveClass, Slot, bSuccessful);
	}
	return nullptr;
}

bool USweetDreamsBPLibrary::SaveCustomGame(const UObject* WorldContext, FString CustomSlot)
{
	if (USweetDreamsCore* Core = GetSweetDreamsCore(WorldContext))
	{
		return Core->Save(CustomSlot);
	}
	return false;
}

bool USweetDreamsBPLibrary::SavePersistentGame(const UObject* WorldContext)
{
	if (USweetDreamsCore* Core = GetSweetDreamsCore(WorldContext))
	{
		FString Slot = Core->GetCoreSaveSlot();
		return Core->Save(Slot);
	}
	return false;
}

USweetDreamsSaveFile* USweetDreamsBPLibrary::LoadCustomGame(const UObject* WorldContext, FString CustomSlot)
{
	if (USweetDreamsCore* Core = GetSweetDreamsCore(WorldContext))
	{
		return Core->LoadSave(CustomSlot);
	}
	return nullptr;
}

USweetDreamsSaveFile* USweetDreamsBPLibrary::LoadPersistentGame(const UObject* WorldContext)
{
	if (USweetDreamsCore* Core = GetSweetDreamsCore(WorldContext))
	{
		FString Slot = Core->GetCoreSaveSlot();
		return Core->LoadSave(Slot);
	}
	return nullptr;
}

bool USweetDreamsBPLibrary::DeleteCustomGame(const UObject* WorldContext, FString CustomSlot)
{
	if (USweetDreamsCore* Core = GetSweetDreamsCore(WorldContext))
	{
		return Core->DeleteSave(CustomSlot);
	}
	return false;
}

bool USweetDreamsBPLibrary::DeletePersistentGame(const UObject* WorldContext)
{
	if (USweetDreamsCore* Core = GetSweetDreamsCore(WorldContext))
	{
		FString Slot = Core->GetCoreSaveSlot();
		return Core->DeleteSave(Slot);
	}
	return false;
}

USweetDreamsSaveFile* USweetDreamsBPLibrary::GetCustomSave(const UObject* WorldContext, FString Slot)
{
	if (USweetDreamsCore* Core = GetSweetDreamsCore(WorldContext))
	{
		return Core->GetSaveObject(Slot);
	}
	return nullptr;
}

USweetDreamsSaveFile* USweetDreamsBPLibrary::GetPersistentSave(const UObject* WorldContext)
{
	if (USweetDreamsCore* Core = GetSweetDreamsCore(WorldContext))
	{
		FString Slot = Core->GetCoreSaveSlot();
		return Core->GetSaveObject(Slot);
	}
	return nullptr;
}

void USweetDreamsBPLibrary::PauseActors(UObject* WorldContext, TArray<AActor*> InActors)
{
	SetTimeDilationActors(WorldContext, InActors, 0.f);
}

void USweetDreamsBPLibrary::PauseAllActors(UObject* WorldContext, TSubclassOf<AActor> IgnoreActorClass)
{
	if (UWorld* World = GetValidWorld(WorldContext))
	{
		for (AActor* Actor : TActorRange<AActor>(World))
		{
			if (!IsValid(Actor) || (IsValid(IgnoreActorClass) && Actor->IsA(IgnoreActorClass))) continue;

			Actor->CustomTimeDilation = 0.f;
		}
	}
}

void USweetDreamsBPLibrary::ResumeActors(UObject* WorldContext, TArray<AActor*> InActors)
{
	SetTimeDilationActors(WorldContext, InActors, 1.f);
}

void USweetDreamsBPLibrary::ResumeAllActors(UObject* WorldContext, TSubclassOf<AActor> IgnoreActorClass)
{
	if (UWorld* World = GetValidWorld(WorldContext))
	{
		for (AActor* Actor : TActorRange<AActor>(World))
		{
			if (!IsValid(Actor) || (IsValid(IgnoreActorClass) && Actor->IsA(IgnoreActorClass))) continue;

			Actor->CustomTimeDilation = 1.f;
		}
	}
}

void USweetDreamsBPLibrary::SetTimeDilationActors(UObject* WorldContext, TArray<AActor*> InActors, const float NewTimeDilation, TSubclassOf<AActor> IgnoreActorClass)
{
	if (UWorld* World = GetValidWorld(WorldContext))
	{
		for (AActor* Actor : InActors)
		{
			if (!IsValid(Actor) || (IsValid(IgnoreActorClass) && Actor->IsA(IgnoreActorClass))) continue;

			Actor->CustomTimeDilation = NewTimeDilation;
		}
	}
}

void USweetDreamsBPLibrary::LoadLevel(const UObject* WorldContext, TSoftObjectPtr<UWorld> Level)
{
	if (USweetDreamsCore* Core = GetSweetDreamsCore(WorldContext))
	{
		Core->LoadLevel(Level);
	}
}

TSoftObjectPtr<UWorld> USweetDreamsBPLibrary::GetCurrentLoadingLevel(const UObject* WorldContext)
{
	if (USweetDreamsCore* Core = GetSweetDreamsCore(WorldContext))
	{
		return Core->CurrentLoadingLevel;
	}
	return nullptr;
}

float USweetDreamsBPLibrary::GetAssetLoadingPercentage(TSoftObjectPtr<UObject> Asset)
{
	if (Asset.IsNull()) return 100.f;
	const FName AssetName = FName(Asset.GetLongPackageName());
	float Percentage = GetAsyncLoadPercentage(AssetName);
	return (Percentage == -1) ? 100.f : Percentage;
}

bool USweetDreamsBPLibrary::CalculateChance(float& RandomizedValue, float Chance)
{
	Chance = FMath::Clamp(Chance / 100.0f, 0.0f, 1.0f);
	RandomizedValue = FMath::FRandRange(0.0f, 1.0f); 
	return RandomizedValue <= Chance;
}

bool USweetDreamsBPLibrary::IsMultipleOf(const float Number, float Interval, float Tolerance)
{
	const float Reminder = FMath::Fmod(Number, Interval);
	return FMath::IsNearlyZero(Reminder, Tolerance);
}

void USweetDreamsBPLibrary::SetGlobalInt(const UObject* WorldContext, FName Key, int32 Value)
{
	if (USweetDreamsCore* Core = GetSweetDreamsCore(WorldContext))
	{
		Core->SetGlobalInt(Key, Value);
	}
}

int32 USweetDreamsBPLibrary::GetGlobalInt(const UObject* WorldContext, FName Key)
{
	if (USweetDreamsCore* Core = GetSweetDreamsCore(WorldContext))
	{
		return Core->GetGlobalInt(Key);
	}

	return INDEX_NONE;
}

void USweetDreamsBPLibrary::SetGlobalBool(const UObject* WorldContext, FName Key, bool Value)
{
	if (USweetDreamsCore* Core = GetSweetDreamsCore(WorldContext))
	{
		Core->SetGlobalBool(Key, Value);
	}
}

bool USweetDreamsBPLibrary::GetGlobalBool(const UObject* WorldContext, FName Key)
{
	if (USweetDreamsCore* Core = GetSweetDreamsCore(WorldContext))
	{
		return Core->GetGlobalBool(Key);
	}

	return false;
}

void USweetDreamsBPLibrary::ServerTravel(UObject* WorldContext, FString MapName, bool bIsListenServer, bool bAbsolute)
{
	if (UWorld* World = GetValidWorld(WorldContext))
	{
		FString Options;
		if (bIsListenServer)
		{
			Options = FString::Printf(TEXT("%s?listen"), *MapName);
		}
		else
		{
			Options = MapName;
		}
		World->ServerTravel(Options, bAbsolute);
	}
}

bool USweetDreamsBPLibrary::GetSessionSetting(FBlueprintSessionResult Result, FName Key, FString& Value)
{
	if (!Result.OnlineResult.IsValid()) return false;
	if (Result.OnlineResult.Session.SessionSettings.Get(Key, Value))
	{
		return true;
	}
	return false;
}

void USweetDreamsBPLibrary::SetSessionSetting(FBlueprintSessionResult Result, FName Key, const FString& Value)
{
	if (!Result.OnlineResult.IsValid()) return;
	Result.OnlineResult.Session.SessionSettings.Set(Key, Value);
}

FNamedOnlineSession* USweetDreamsBPLibrary::GetCurrentSession(UObject* WorldContext)
{
	UWorld* const World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::LogAndReturnNull);
	IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(World);
	if (!SessionInterface.IsValid()) return nullptr;
	FNamedOnlineSession* Session = SessionInterface->GetNamedSession(NAME_GameSession);
	if (Session != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Session found."));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Session nullptr."));
	}
	return Session;
}

bool USweetDreamsBPLibrary::GetCurrentSessionSetting(UObject* WorldContext, FName Key, FString& Value)
{
	FNamedOnlineSession* Session = GetCurrentSession(WorldContext);
	if (!Session)
	{
		return false;
	}
	return Session->SessionSettings.Get(Key, Value);
}

void USweetDreamsBPLibrary::SetCurrentSessionSetting(UObject* WorldContext, FName Key, const FString& Value)
{
	FNamedOnlineSession* Session = GetCurrentSession(WorldContext);
	if (!Session)
	{
		return;
	}
	Session->SessionSettings.Set(Key, Value, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
}

void USweetDreamsBPLibrary::RenameSession(UObject* WorldContext, const FString& NewName, int32 MaxLength)
{
	if (NewName.Len() > MaxLength || MaxLength < 0) return;
	SetCurrentSessionSetting(WorldContext, FName(FString("SERVER_NAME")), NewName);
}

bool USweetDreamsBPLibrary::GetCurrentSessionName(UObject* WorldContext, FString& SessionName)
{
	if (GetCurrentSessionSetting(WorldContext, FName(FString("SERVER_NAME")), SessionName))
	{
		return true;
	}
	return false;
}

void USweetDreamsBPLibrary::SetSessionPlayerReady(UObject* WorldContext, const FString& PlayerID, bool bReady)
{
	SetCurrentSessionSetting(WorldContext, FName(FString("PLAYER_READY_" + PlayerID)), bReady ? TEXT("True") : TEXT("False"));
}

bool USweetDreamsBPLibrary::IsPlayerReady(UObject* WorldContext, const FString& PlayerID)
{
	FString Value;
	if (GetCurrentSessionSetting(WorldContext, FName(FString("PLAYER_READY_") + PlayerID), Value))
	{
		return Value.Equals(TEXT("True"), ESearchCase::IgnoreCase);
	}
	return false;
}

bool USweetDreamsBPLibrary::ArePlayersReady(UObject* WorldContext)
{
	FNamedOnlineSession* Session = GetCurrentSession(WorldContext);
	if (!Session) return false;
	bool bReady = false;
	const FSessionSettings& Settings = Session->SessionSettings.Settings;
	for (const TPair<FName, FOnlineSessionSetting>& Pair : Settings)
	{
		const FString KeyStr = Pair.Key.ToString();
		if (KeyStr.StartsWith(TEXT("PLAYER_READY_")))
		{
			bReady = true;
			FString Value;
			if (!Session->SessionSettings.Get(Pair.Key, Value) || !Value.Equals(TEXT("True"), ESearchCase::IgnoreCase))
			{
				return false;
			}
		}
	}
	return bReady;
}


FString USweetDreamsBPLibrary::GetPlayerUniqueId(APlayerController* PlayerController)
{
	const FUniqueNetIdRepl UniqueID = PlayerController->PlayerState->GetUniqueId();
	if (UniqueID.IsValid())
	{
		return UniqueID->ToString();
	}
	return FString();
}

void USweetDreamsBPLibrary::ShouldNotHappen(const UObject* WorldContext)
{
	LogDream(WorldContext, "Should NOT happen called.", EPrintType::Error, 10.f);
}

void USweetDreamsBPLibrary::DoSomething(const UObject* WorldContext)
{
	LogDream(WorldContext, "Something has been done.", EPrintType::Warning, 10.f);
}

bool USweetDreamsBPLibrary::IsRunningInEditor()
{
#if WITH_EDITOR
	return GIsEditor;
#else
	return false;
#endif
}

bool USweetDreamsBPLibrary::IsRunningInStandaloneGame()
{
	return !IsRunningInEditor();
}

void USweetDreamsBPLibrary::LogDream(const UObject* WorldContext, FString Log, EPrintType Severity, float Duration, bool bLogToScreen)
{
	if (USweetDreamsCore* Core = GetSweetDreamsCore(WorldContext))
	{
		Core->Log(WorldContext, Log, Severity, Duration, bLogToScreen);
	}
}
