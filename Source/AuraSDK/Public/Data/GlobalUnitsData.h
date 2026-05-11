#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "GlobalUnitsData.generated.h"

// A data asset meant to store easy-to-access integers assigned by an unique Gameplay Tag.
// You can use the GetGlobalUnitValue() to get the integer of the specified tag and data asset.

UCLASS(BlueprintType)
class AURASDK_API UGlobalUnitsData : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Global Units")
	TMap<FGameplayTag, int32> Units;

	UFUNCTION(BlueprintPure, Category = "AuraSDK | Global Units Data")
	static int32 GetGlobalUnitValue(const UGlobalUnitsData* Data, const FGameplayTag& Tag)
	{
		if (Data)
		{
			if (const int32* Value = Data->Units.Find(Tag))
			{
				return *Value;
			}
		}

		return INDEX_NONE;
	}

};