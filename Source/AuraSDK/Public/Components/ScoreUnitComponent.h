

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "ScoreUnitComponent.generated.h"

USTRUCT(BlueprintType)
struct AURASDK_API FScoreUnit
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintReadOnly, SaveGame, Category = "Score Unit")
	int32 Value = 0.f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, SaveGame, Category = "Score Unit")
	int32 MaxValue = 0.f;

	FScoreUnit() = default;
};


UCLASS( ClassGroup=("AuraSDK"), meta=(BlueprintSpawnableComponent) )
class AURASDK_API UScoreUnitComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UScoreUnitComponent();

	virtual void Serialize(FArchive& Ar) override;

	UFUNCTION(BlueprintCallable, Category = "AuraSDK | Score Unit Component")
	void IncreaseScore(const FGameplayTag& Score, int32 Value);
	UFUNCTION(BlueprintCallable, Category = "AuraSDK | Score Unit Component")
	void DecreaseScore(const FGameplayTag& Score, int32 Value);
	UFUNCTION(BlueprintCallable, Category = "AuraSDK | Score Unit Component")
	void ModifyScore(const FGameplayTag& Score, int32 Value);

	UFUNCTION(BlueprintCallable, Category = "AuraSDK | Score Unit Component")
	bool IsScoreGreaterThan(const FGameplayTag& Score, int32 Value) const;
	UFUNCTION(BlueprintCallable, Category = "AuraSDK | Score Unit Component")
	int32 GetScoreValue(const FGameplayTag& Score) const;

protected:

	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Score Unit", SaveGame)
	TMap<FGameplayTag, FScoreUnit> Scores;

};
