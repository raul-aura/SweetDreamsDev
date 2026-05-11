#pragma once

#include "Subsystems/GameInstanceSubsystem.h"
#include "LevelManager.generated.h"

class ULevelData;
class ULevelResolver;
class ULevelCompletionComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLevelManagerDelegate);

UCLASS()
class AURASDK_API ULevelManager final : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable, Category = "AuraSDK | Level Manager")
	void UpdateLevelLibrary(TArray<ULevelData*> InLevels);
	UFUNCTION(BlueprintCallable, Category = "AuraSDK | Level Manager")
	void CreateLevelResolver(TSubclassOf<ULevelResolver> ResolverClass);
	UFUNCTION(BlueprintCallable, Category = "AuraSDK | Level Manager")
	void LoadNextLevel();
	UFUNCTION(BlueprintCallable, Category = "AuraSDK | Level Manager")
	void TeleportToNextLevel();

	void RegisterExitComponent(ULevelCompletionComponent* Component);

	UFUNCTION(BlueprintCallable, Category = "AuraSDK | Level Manager")
	ULevelData* GetLevelDataFromName(FName InName) const;
	UFUNCTION(BlueprintCallable, Category = "AuraSDK | Level Manager")
	ULevelResolver* GetLevelResolver() const;
	UFUNCTION(BlueprintCallable, Category = "AuraSDK | Level Manager")
	ULevelData* GetCurrentLevel() const;

	UPROPERTY(BlueprintAssignable)
	FLevelManagerDelegate OnLevelFinishLoading;

private:

	TArray<ULevelData*> Levels;
	ULevelData* CurrentLevel = nullptr;

	TObjectPtr<ULevelResolver> LevelResolver = nullptr;
};
