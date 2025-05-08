// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/BattlerDataComponent.h"
#include "Data/SweetDreamsLevel.h"
#include "Core/SweetDreamsBPLibrary.h"

UBattlerDataComponent::UBattlerDataComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	LevelClass = USweetDreamsLevel::StaticClass();
}

void UBattlerDataComponent::BeginPlay()
{
	CurrentLives = FMath::Max(1, AdditionalLives + 1);
	if (IsValid(LevelClass))
	{
		LevelObject = NewObject<USweetDreamsLevel>(this, LevelClass);
	}
	Super::BeginPlay();
}

void UBattlerDataComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

UBattlerDataComponent* UBattlerDataComponent::GetBattlerDataComponent(const AActor* Actor)
{
	UWorld* World = GEngine->GetWorldFromContextObject(Actor, EGetWorldErrorMode::ReturnNull);
	if (!IsValid(Actor) && !IsValid(World))
	{
		return nullptr;
	}
	UBattlerDataComponent* Component = Actor->FindComponentByClass<UBattlerDataComponent>();
	return Component;
}

FText UBattlerDataComponent::GetCharacterName() const
{
	return BattlerName;
}

void UBattlerDataComponent::SetCharacterName(FText NewName)
{
	BattlerName = NewName;
}

int32 UBattlerDataComponent::GetLevelNumber() const
{
	if (IsValid(LevelObject))
	{
		return LevelObject->GetCurrentLevel();
	}
	return -1;
}

void UBattlerDataComponent::SetLevelNumber(int32 NewLevel)
{
	if (IsValid(LevelObject))
	{
		LevelObject->SetCurrentLevel(NewLevel);
	}
}

void UBattlerDataComponent::Kill(AActor* KillInstigator)
{
	bIsDead = true;
	CurrentLives = FMath::Clamp(--CurrentLives, 0, AdditionalLives);
	if (CurrentLives > 0)
	{
		if (ReviveDelay == 0.f)
		{
			return Revive();
		}
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
			{
				Revive();
			}, ReviveDelay, false);
		return;
	}
}

void UBattlerDataComponent::Revive(float HealthRestore, float ManaRestore)
{
	bIsDead = false;
}

bool UBattlerDataComponent::IsDead() const
{
	return bIsDead;
}

bool UBattlerDataComponent::IsInBattle() const
{
	return bIsInBattle;
}

void UBattlerDataComponent::SetInBattle(ASweetDreamsBattleManager* BattleReference, bool bNewIsInBattle)
{
	bIsInBattle = bNewIsInBattle;
	if (bIsInBattle)
	{
	}
}


