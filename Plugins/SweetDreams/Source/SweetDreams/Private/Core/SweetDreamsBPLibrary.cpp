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

USweetDreamsCore* USweetDreamsBPLibrary::SweetDreamsCore = nullptr;

USweetDreamsBPLibrary::USweetDreamsBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
}

USweetDreamsCore* USweetDreamsBPLibrary::GetSweetDreamsCore(const UObject* WorldContext)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::ReturnNull);
	if (!IsValid(WorldContext) && !IsValid(World))
	{
		return nullptr;
	}
	if (UGameInstance* GameInstance = World->GetGameInstance())
	{
		SweetDreamsCore = GameInstance->GetSubsystem<USweetDreamsCore>();
	}
	return SweetDreamsCore;
}

ASweetDreamsGameMode* USweetDreamsBPLibrary::GetSweetDreamsGameMode(const UObject* WorldContext)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::ReturnNull);
	if (!IsValid(WorldContext) && !IsValid(World))
	{
		return nullptr;
	}
	return Cast<ASweetDreamsGameMode>(UGameplayStatics::GetGameMode(World));
}

bool USweetDreamsBPLibrary::CreateSaveGame(const UObject* WorldContext, TSubclassOf<USweetDreamsSaveFile> SaveClass, bool bIsPersistent)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::ReturnNull);
	if (!IsValid(WorldContext) && !IsValid(World))
	{
		return false;
	}
	if (UGameInstance* GameInstance = World->GetGameInstance())
	{
		SweetDreamsCore = GameInstance->GetSubsystem<USweetDreamsCore>();
		if (IsValid(SweetDreamsCore))
		{
			return SweetDreamsCore->CreateSave(SaveClass, bIsPersistent);
		}
	}
	return false;
}

bool USweetDreamsBPLibrary::SaveGame(const UObject* WorldContext, USweetDreamsSaveFile* SaveObject, bool bIsPersistent)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::ReturnNull);
	if (!IsValid(WorldContext) && !IsValid(World))
	{
		return false;
	}
	if (UGameInstance* GameInstance = World->GetGameInstance())
	{
		SweetDreamsCore = GameInstance->GetSubsystem<USweetDreamsCore>();
		if (IsValid(SweetDreamsCore))
		{
			return SweetDreamsCore->Save(SaveObject, bIsPersistent);
		}
	}
	return false;
}

USweetDreamsSaveFile* USweetDreamsBPLibrary::LoadSaveGame(const UObject* WorldContext, bool bIsPersistent)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::ReturnNull);
	if (!IsValid(WorldContext) && !IsValid(World))
	{
		return nullptr;
	}
	if (UGameInstance* GameInstance = World->GetGameInstance())
	{
		SweetDreamsCore = GameInstance->GetSubsystem<USweetDreamsCore>();
		if (IsValid(SweetDreamsCore))
		{
			return SweetDreamsCore->LoadSave(bIsPersistent);
		}
	}
	return nullptr;
}

USweetDreamsSaveFile* USweetDreamsBPLibrary::GetPersistentSave(const UObject* WorldContext)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::ReturnNull);
	if (!IsValid(WorldContext) && !IsValid(World))
	{
		return nullptr;
	}
	if (UGameInstance* GameInstance = World->GetGameInstance())
	{
		SweetDreamsCore = GameInstance->GetSubsystem<USweetDreamsCore>();
		if (IsValid(SweetDreamsCore))
		{
			return SweetDreamsCore->SavePersistentRef;
		}
	}
	return nullptr;
}

USweetDreamsSaveFile* USweetDreamsBPLibrary::GetLocalSave(const UObject* WorldContext)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::ReturnNull);
	if (!IsValid(WorldContext) && !IsValid(World))
	{
		return nullptr;
	}
	if (UGameInstance* GameInstance = World->GetGameInstance())
	{
		SweetDreamsCore = GameInstance->GetSubsystem<USweetDreamsCore>();
		if (IsValid(SweetDreamsCore))
		{
			return SweetDreamsCore->SaveLocalRef;
		}
	}
	return nullptr;
}

bool USweetDreamsBPLibrary::DeleteSave(const UObject* WorldContext, bool bIsPersistent)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::ReturnNull);
	if (!IsValid(WorldContext) && !IsValid(World))
	{
		return false;
	}
	if (UGameInstance* GameInstance = World->GetGameInstance())
	{
		SweetDreamsCore = GameInstance->GetSubsystem<USweetDreamsCore>();
		if (IsValid(SweetDreamsCore))
		{
			return SweetDreamsCore->DeleteSave(bIsPersistent);
		}
	}
	return false;
}

FDreamUserSettings USweetDreamsBPLibrary::GetUserSettings(const UObject* WorldContext)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::ReturnNull);
	if (!IsValid(WorldContext) && !IsValid(World))
	{
		return FDreamUserSettings();
	}
	if (UGameInstance* GameInstance = World->GetGameInstance())
	{
		SweetDreamsCore = GameInstance->GetSubsystem<USweetDreamsCore>();
		if (IsValid(SweetDreamsCore))
		{
			FDreamUserSettings Settings = SweetDreamsCore->GetUserSettings();
			return Settings;
		}
	}
	return FDreamUserSettings();
}

void USweetDreamsBPLibrary::SetUserSettings(const UObject* WorldContext, FDreamUserSettings Settings)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::ReturnNull);
	if (!IsValid(WorldContext) && !IsValid(World))
	{
		return;
	}
	if (UGameInstance* GameInstance = World->GetGameInstance())
	{
		SweetDreamsCore = GameInstance->GetSubsystem<USweetDreamsCore>();
		if (IsValid(SweetDreamsCore))
		{
			Settings.ApplySettings();
			SweetDreamsCore->SetUserSettings(Settings);
		}
	}
}

void USweetDreamsBPLibrary::SetSettingsQuality(const UObject* WorldContext, int32 Quality)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::ReturnNull);
	if (!IsValid(WorldContext) && !IsValid(World))
	{
		return;
	}
	if (UGameInstance* GameInstance = World->GetGameInstance())
	{
		SweetDreamsCore = GameInstance->GetSubsystem<USweetDreamsCore>();
		if (IsValid(SweetDreamsCore))
		{
			Quality = FMath::Clamp(Quality, 0, 2);
			FDreamUserSettings NewSettings(Quality);
			NewSettings.ApplySettings();
			SweetDreamsCore->SetUserSettings(NewSettings);
		}
	}
}

void USweetDreamsBPLibrary::LoadLevel(const UObject* WorldContext, TSoftObjectPtr<UWorld> Level)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::ReturnNull);
	if (!IsValid(WorldContext) && !IsValid(World))
	{
		return;
	}
	GetSweetDreamsCore(WorldContext)->LoadLevel(Level);
}

TSoftObjectPtr<UWorld> USweetDreamsBPLibrary::GetCurrentLoadingLevel(const UObject* WorldContext)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::ReturnNull);
	if (!IsValid(WorldContext) && !IsValid(World))
	{
		return nullptr;
	}
	return GetSweetDreamsCore(WorldContext)->CurrentLoadingLevel;
}

float USweetDreamsBPLibrary::GetLoadingPercentage(TSoftObjectPtr<UObject> Asset)
{
	if (Asset.IsNull()) return 100.f;
	const FName AssetName = FName(Asset.GetLongPackageName());
	float Percentage = GetAsyncLoadPercentage(AssetName);
	return (Percentage == -1) ? 100.f : Percentage;
}

float USweetDreamsBPLibrary::IncrementAlpha(const UObject* WorldContext, UPARAM(ref) float& Alpha, float MaxValue, UCurveFloat* AlphaCurve, bool bStopCondition)
{
	return 0.0f;
}

bool USweetDreamsBPLibrary::CalculateChance(float& RandomizedValue, float Chance)
{
	Chance = FMath::Clamp(Chance / 100.0f, 0.0f, 1.0f);
	RandomizedValue = FMath::FRandRange(0.0f, 1.0f); 
	return RandomizedValue <= Chance;
}

bool USweetDreamsBPLibrary::CheckInterval(const float& Number, float Interval)
{
	float Reminder = FMath::Fmod(Number, Interval);
	return FMath::IsNearlyZero(Reminder, 0.01f);
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
	UWorld* World = GEngine->GetWorldFromContextObject(DreamOrigin, EGetWorldErrorMode::ReturnNull);
	if (!IsValid(DreamOrigin) && !IsValid(World))
	{
		return;
	}
	GetSweetDreamsCore(DreamOrigin)->PrintDream(DreamOrigin, Dream, Severity, Duration);
}
