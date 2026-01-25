// Fill out your copyright notice in the Description page of Project Settings.

#include "Battle/SweetDreamsBattleCore.h"

USweetDreamsBattleCore::USweetDreamsBattleCore() {}

void USweetDreamsBattleCore::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

int32 USweetDreamsBattleCore::GetDifficulty() const
{
	return BattleDifficulty;
}

void USweetDreamsBattleCore::SetDifficulty(int32 NewDifficulty)
{
	BattleDifficulty = FMath::Max(NewDifficulty, 0);
}
