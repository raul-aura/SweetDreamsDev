// Copyright Epic Games, Inc. All Rights Reserved.

#include "Core/SweetDreamsBPLibrary.h"
#include "Core/SweetDreams.h"
#include "Core/SweetDreamsCore.h"
#include "UMG/Public/Components/PanelWidget.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "Game/SweetDreamsGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Curves/CurveFloat.h"
#include "Engine/Engine.h"

USweetDreamsBPLibrary::USweetDreamsBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
}

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
		FString Slot = Core->GetCoreSaveSlot(true);
		return Core->CreateSave(SaveClass, Slot, bSuccessful);
	}
	return nullptr;
}

USweetDreamsSaveFile* USweetDreamsBPLibrary::CreateLocalSave(const UObject* WorldContext, TSubclassOf<USweetDreamsSaveFile> SaveClass, bool& bSuccessful)
{
	if (USweetDreamsCore* Core = GetSweetDreamsCore(WorldContext))
	{
		FString Slot = Core->GetCoreSaveSlot(false);
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
		FString Slot = Core->GetCoreSaveSlot(true);
		return Core->Save(Slot);
	}
	return false;
}

bool USweetDreamsBPLibrary::SaveLocalGame(const UObject* WorldContext)
{
	if (USweetDreamsCore* Core = GetSweetDreamsCore(WorldContext))
	{
		FString Slot = Core->GetCoreSaveSlot(false);
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
		FString Slot = Core->GetCoreSaveSlot(true);
		return Core->LoadSave(Slot);
	}
	return nullptr;
}

USweetDreamsSaveFile* USweetDreamsBPLibrary::LoadLocalGame(const UObject* WorldContext)
{
	if (USweetDreamsCore* Core = GetSweetDreamsCore(WorldContext))
	{
		FString Slot = Core->GetCoreSaveSlot(false);
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
		FString Slot = Core->GetCoreSaveSlot(true);
		return Core->DeleteSave(Slot);
	}
	return false;
}

bool USweetDreamsBPLibrary::DeleteLocalGame(const UObject* WorldContext)
{
	if (USweetDreamsCore* Core = GetSweetDreamsCore(WorldContext))
	{
		FString Slot = Core->GetCoreSaveSlot(false);
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
		FString Slot = Core->GetCoreSaveSlot(true);
		return Core->GetSaveObject(Slot);
	}
	return false;
}

USweetDreamsSaveFile* USweetDreamsBPLibrary::GetLocalSave(const UObject* WorldContext)
{
	if (USweetDreamsCore* Core = GetSweetDreamsCore(WorldContext))
	{
		FString Slot = Core->GetCoreSaveSlot(false);
		return Core->GetSaveObject(Slot);
	}
	return false;
}

FDreamUserSettings USweetDreamsBPLibrary::GetUserSettings(const UObject* WorldContext)
{
	if (USweetDreamsCore* Core = GetSweetDreamsCore(WorldContext))
	{
		return Core->GetUserSettings();
	}
	return FDreamUserSettings();
}

void USweetDreamsBPLibrary::SetUserSettings(const UObject* WorldContext, FDreamUserSettings Settings)
{
	if (USweetDreamsCore* Core = GetSweetDreamsCore(WorldContext))
	{
		Settings.ApplySettings();
		Core->SetUserSettings(Settings);
	}
}

void USweetDreamsBPLibrary::SetSettingsQuality(const UObject* WorldContext, int32 Quality)
{
	if (USweetDreamsCore* Core = GetSweetDreamsCore(WorldContext))
	{
		Quality = FMath::Clamp(Quality, 0, 2);
		FDreamUserSettings NewSettings(Quality);
		NewSettings.ApplySettings();
		Core->SetUserSettings(NewSettings);
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

bool USweetDreamsBPLibrary::IsMultipleOf(const float& Number, float Interval, float Tolerance = 0.01f)
{
	float Reminder = FMath::Fmod(Number, Interval);
	return FMath::IsNearlyZero(Reminder, Tolerance);
}

void USweetDreamsBPLibrary::ShouldNotHappen(const UObject* WorldContext)
{
	PrintDream(WorldContext, "Should NOT happen called.", EPrintType::ERROR, 10.f);
}

void USweetDreamsBPLibrary::DoSomething(const UObject* WorldContext)
{
	PrintDream(WorldContext, "Something has been done.", EPrintType::WARNING, 10.f);
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

void USweetDreamsBPLibrary::PrintDream(const UObject* DreamOrigin, FString Dream, EPrintType Severity, float Duration)
{
	if (USweetDreamsCore* Core = GetSweetDreamsCore(DreamOrigin))
	{
		Core->PrintDream(DreamOrigin, Dream, Severity, Duration);
	}
}
