
#include "Components/ScoreUnitComponent.h"

UScoreUnitComponent::UScoreUnitComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UScoreUnitComponent::Serialize(FArchive& Ar)
{
	Super::Serialize(Ar);

	if (Ar.IsSaving())
	{
		if (Ar.IsSaveGame())
		{
			UE_LOG(LogTemp, Warning, TEXT("Component %s is being serialized with SaveGame flag"), *GetName());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Component %s is NOT being serialized with flag"), *GetName());
		}
	}
}

void UScoreUnitComponent::IncreaseScore(const FGameplayTag& Score, int32 Value)
{
	if (FScoreUnit* Data = Scores.Find(Score))
	{
		Data->Value = FMath::Min(Data->Value + Value, Data->MaxValue);
	}
}

void UScoreUnitComponent::DecreaseScore(const FGameplayTag& Score, int32 Value)
{
	if (FScoreUnit* Data = Scores.Find(Score))
	{
		Data->Value = FMath::Max(Data->Value - Value, 0);
	}
}

void UScoreUnitComponent::ModifyScore(const FGameplayTag& Score, int32 Value)
{
	if (FScoreUnit* Data = Scores.Find(Score))
	{
		Data->Value = FMath::Min(Value, Data->MaxValue);
	}
}

bool UScoreUnitComponent::IsScoreGreaterThan(const FGameplayTag& Score, int32 Value) const
{
	if (const FScoreUnit* Data = Scores.Find(Score))
	{
		return Value >= Data->Value;
	}

	return false;
}

int32 UScoreUnitComponent::GetScoreValue(const FGameplayTag& Score) const
{
	if (const FScoreUnit* Data = Scores.Find(Score))
	{
		return Data->Value;
	}

	return INDEX_NONE;
}

void UScoreUnitComponent::BeginPlay()
{
	Super::BeginPlay();
}


