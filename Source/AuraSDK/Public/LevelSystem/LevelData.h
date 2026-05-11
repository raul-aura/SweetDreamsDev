#pragma once
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LevelData.generated.h"

UCLASS()
class AURASDK_API ULevelData : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Level")
	FName LevelIdentifier;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Level")
	TSoftObjectPtr<UWorld> LevelReference;
};

