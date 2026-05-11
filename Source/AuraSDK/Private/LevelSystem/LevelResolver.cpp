
#include "LevelSystem/LevelResolver.h"

FName ULevelResolver::ResolveNextLevel_Implementation(const FName& CurrentLevel) const
{
	return NAME_None;
}

void ULevelResolver::RegisterLevel(const FName& Level) const
{
	Visited.Add(Level);
}

void ULevelResolver::ClearVisitedLevels()
{
	Visited.Empty();
}

FName ULinearLevelResolver::ResolveNextLevel_Implementation(const FName& CurrentLevel) const
{
	RegisterLevel(CurrentLevel);

	TArray<FName> Pool = OrderedLevels;

	if (bIgnoreVisitedLevels)
	{
		Pool = GetNonVisitedLevels(OrderedLevels);
	}

	const int32 Index = Pool.IndexOfByKey(CurrentLevel);
	return Pool.IsValidIndex(Index + 1) ? Pool[Index + 1] : NAME_None;
}

FName URandomLevelResolver::ResolveNextLevel_Implementation(const FName& CurrentLevel) const
{
	RegisterLevel(CurrentLevel);

	TArray<FName> Pool = PossibleLevels;

	if (bIgnoreVisitedLevels)
	{
		Pool = GetNonVisitedLevels(PossibleLevels);
	}

	return Pool.Num() > 0 ? Pool[FMath::RandRange(0, Pool.Num() - 1)] : NAME_None;
}

TArray<FName> ULevelResolver::GetNonVisitedLevels(const TArray<FName>& Pool) const
{
	TArray<FName> Result;
	Result.Reserve(Pool.Num());

	for (const FName& Level : Pool)
	{
		if (!Visited.Contains(Level))
		{
			Result.Add(Level);
		}
	}

	return Result;
}


