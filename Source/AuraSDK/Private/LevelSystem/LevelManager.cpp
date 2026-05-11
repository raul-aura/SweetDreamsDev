

#include "LevelSystem/LevelManager.h"
#include "LevelSystem/LevelData.h"
#include "LevelSystem/LevelResolver.h"
#include "LevelSystem/LevelCompletionComponent.h"
#include "Kismet/GameplayStatics.h"

//SWEET DREAMS
#include "Core/SweetDreamsBPLibrary.h"
#include "AuraSDK.h"

//ASSET MANAGER
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"


void ULevelManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void ULevelManager::Deinitialize()
{
	Super::Deinitialize();
}

void ULevelManager::UpdateLevelLibrary(TArray<ULevelData*> InLevels)
{
	Levels = InLevels;
}

void ULevelManager::CreateLevelResolver(TSubclassOf<ULevelResolver> ResolverClass)
{
	if (!LevelResolver)
	{
		LevelResolver = NewObject<ULevelResolver>(this, ResolverClass);
	}
}

void ULevelManager::LoadNextLevel()
{
	if (LevelResolver)
	{
		FName NextLevel = LevelResolver->ResolveNextLevel(CurrentLevel->LevelIdentifier);

		if (!NextLevel.IsNone())
		{
			CurrentLevel = GetLevelDataFromName(NextLevel);
			TSoftObjectPtr<UWorld> LevelObject = nullptr;

			if (CurrentLevel)
			{
				LevelObject = CurrentLevel->LevelReference;
			}

			if (LevelObject.IsValid())
			{
				if (SweetDreams::IsLoaded())
				{
					USweetDreamsBPLibrary::LoadLevel(this, LevelObject);
				}
				else
				{
					FStreamableManager& Streamable = UAssetManager::GetStreamableManager();

					Streamable.RequestAsyncLoad(LevelObject.ToSoftObjectPath(), 
						FStreamableDelegate::CreateUObject(this, &ULevelManager::TeleportToNextLevel)
					);
				}
			}
		}
	}
}

void ULevelManager::TeleportToNextLevel()
{
	UWorld* NextLevel = CurrentLevel->LevelReference.Get();

	if (NextLevel)
	{
		UGameplayStatics::OpenLevelBySoftObjectPtr(this, NextLevel);
	}
}

void ULevelManager::RegisterExitComponent(ULevelCompletionComponent* Component)
{
	if (Component)
	{
		Component->OnLevelExit.AddUniqueDynamic(this, &ULevelManager::LoadNextLevel);
	}
}

ULevelData* ULevelManager::GetLevelDataFromName(FName InName) const
{
	for (ULevelData* Level : Levels)
	{
		if (Level && Level->LevelIdentifier.IsEqual(InName))
		{
			return Level;
		}
	}

	return nullptr;
}

ULevelResolver* ULevelManager::GetLevelResolver() const
{
	return LevelResolver;
}

ULevelData* ULevelManager::GetCurrentLevel() const
{
	return CurrentLevel;
}