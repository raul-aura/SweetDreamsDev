

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "LevelResolver.generated.h"

UCLASS(BlueprintType, Blueprintable, Abstract, EditInlineNew)
class AURASDK_API ULevelResolver : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent)
	FName ResolveNextLevel(const FName& CurrentLevel) const;
	virtual FName ResolveNextLevel_Implementation(const FName& CurrentLevel) const;

	void RegisterLevel(const FName& Level) const;
	UFUNCTION(BlueprintCallable, Category = "AuraSDK | Level Resolver")
	void ClearVisitedLevels();

	TArray<FName> GetNonVisitedLevels(const TArray<FName>& Pool) const;

	UPROPERTY(Transient)
	mutable TSet<FName> Visited;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Level Resolver")
	bool bIgnoreVisitedLevels = true;
};

UCLASS()
class AURASDK_API ULinearLevelResolver : public ULevelResolver
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	TArray<FName> OrderedLevels;

	virtual FName ResolveNextLevel_Implementation(const FName& CurrentLevel) const override;
};

UCLASS()
class AURASDK_API URandomLevelResolver : public ULevelResolver
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	TArray<FName> PossibleLevels;

	virtual FName ResolveNextLevel_Implementation(const FName& CurrentLevel) const override;
};