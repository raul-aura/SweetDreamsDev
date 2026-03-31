
#include "Data/BattleContext.h"
#include "Data/BattleElement.h"
#include "Battle/BattleActorComponent.h"
#include "Battle/SweetDreamsBattleInterface.h"

#include "Algo/RandomShuffle.h"
#include "Kismet/GameplayStatics.h"

void UBattleContext::Initialize(UBattleElement* Owner, TObjectPtr<UBattleActorComponent> InInstigator, TArray<UBattleActorComponent*> InCandidates)
{
	Instigator = InInstigator;
	OwnerElement = Owner;

	UpdateCandidates(InCandidates);
}

void UBattleContext::UpdateCandidates(TArray<UBattleActorComponent*> InCandidates)
{
	CandidateActors = InCandidates;
}

UBattleElement* UBattleContext::GetOwnerElement() const
{
	return OwnerElement.Get();
}

UBattleActorComponent* UBattleContext::GetInstigator() const
{
	return Instigator;
}

FDamageHealResult UBattleContext::Damage(float Value, FGameplayTagContainer EffectTags, const FSelectedTargetsSettings& Settings)
{
	const float Damage = FMath::Abs(Value);

	FDamageHealResult Result(Instigator, EffectTags);

	const TArray<UBattleActorComponent*> Targets = GetSelectedTargets(Settings);

	for (UBattleActorComponent* Target : Targets)
	{
		if (!IsValid(Target))
		{
			continue;
		}

		FDamageHealTargetResult TargetResult = Target->ReceiveDamage(Instigator, EffectTags, Damage);

		if (TargetResult.Target)
		{
			Result.Targets.Add(TargetResult);
		}
	}

	if (IsValid(Result.Instigator))
	{
		Result.Instigator->OnDamageDealt.Broadcast(Result.Instigator, Result);
	}

	return Result;
}

FDamageHealResult UBattleContext::Heal(float Value, FGameplayTagContainer EffectTags, const FSelectedTargetsSettings& Settings)
{
	const float Heal = FMath::Abs(Value);

	FDamageHealResult Result(Instigator, EffectTags);

	const TArray<UBattleActorComponent*> Targets = GetSelectedTargets(Settings);

	for (UBattleActorComponent* Target : Targets)
	{
		if (!IsValid(Target))
		{
			continue;
		}

		FDamageHealTargetResult TargetResult = Target->ReceiveHeal(Instigator, EffectTags, Heal);

		if (TargetResult.Target)
		{
			Result.Targets.Add(TargetResult);
		}
	}

	if (IsValid(Result.Instigator))
	{
		Result.Instigator->OnHealingDealt.Broadcast(Result.Instigator, Result);
	}

	return Result;
}

void UBattleContext::Kill(const FSelectedTargetsSettings& Settings)
{
	const TArray<UBattleActorComponent*> Targets = GetSelectedTargets(Settings);

	for (UBattleActorComponent* Target : Targets)
	{
		Target->SetIsAlive(false);
	}
}

void UBattleContext::Ressurect(const FSelectedTargetsSettings& Settings)
{
	const TArray<UBattleActorComponent*> Targets = GetSelectedTargets(Settings);

	for (UBattleActorComponent* Target : Targets)
	{
		Target->SetIsAlive(true);
	}
}

FBattleParameterModifier UBattleContext::AddModifierToParameter(FGameplayTag ParameterTag, EParameterModifierType ModifierType, float ModifierValue, const FSelectedTargetsSettings& Settings)
{
	const TArray<UBattleActorComponent*> Targets = GetSelectedTargets(Settings);

	FBattleParameterModifier Modifier;

	for (UBattleActorComponent* Target : Targets)
	{
		Modifier = Target->AddModifierToParameter(ParameterTag, ModifierType, ModifierValue);
	}

	return Modifier;
}

void UBattleContext::RemoveModifierFromParameter(FGameplayTag ParameterTag, const FBattleParameterModifier& Modifier, const FSelectedTargetsSettings& Settings)
{
	const TArray<UBattleActorComponent*> Targets = GetSelectedTargets(Settings);

	for (UBattleActorComponent* Target : Targets)
	{
		Target->RemoveModifierFromParameter(ParameterTag, Modifier);
	}
}

void UBattleContext::IncreaseParameterResource(FGameplayTag ParameterTag, float Value, const FSelectedTargetsSettings& Settings)
{
	const TArray<UBattleActorComponent*> Targets = GetSelectedTargets(Settings);

	for (UBattleActorComponent* Target : Targets)
	{
		Target->IncreaseParameterResource(ParameterTag, Value);
	}
}

void UBattleContext::DecreaseParameterResource(FGameplayTag ParameterTag, float Value, const FSelectedTargetsSettings& Settings)
{
	const TArray<UBattleActorComponent*> Targets = GetSelectedTargets(Settings);

	for (UBattleActorComponent* Target : Targets)
	{
		Target->DecreaseParameterResource(ParameterTag, Value);
	}
}

AActor* UBattleContext::SpawnActor(TSubclassOf<AActor> ActorClass, FTransform Transform)
{
	if (UWorld* World = GetInstigator()->GetWorld())
	{
		AActor* Spawned = World->SpawnActorDeferred<AActor>(ActorClass, Transform);

		if (Spawned)
		{
			ISweetDreamsBattleInterface::Execute_SendBattleActorComponent(Spawned, GetInstigator());

			UGameplayStatics::FinishSpawningActor(Spawned, Transform);

			return Spawned;
		}
	}

	return nullptr;
}

void UBattleContext::RequestBattleElementEnd()
{
	if (UBattleElement* Element = GetOwner())
	{
		Element->End();
	}
}

float UBattleContext::GetDeltaTime() const
{
	if (const UBattleElement* Element = GetOwner())
	{
		return Element->CachedDeltaTime;
	}

	return 0.f;
}

TArray<UBattleActorComponent*> UBattleContext::GetSelectedTargets(const FSelectedTargetsSettings& Settings) const
{
	TArray<UBattleActorComponent*> SelectedTargets;
	TArray<UBattleActorComponent*> CachedTargets = CandidateActors;

	switch (Settings.TargetType)
	{
	case ETargetSelectionScope::Candidates:
		break;
	case ETargetSelectionScope::CandidatesAndInstigator:
		CachedTargets.Add(Instigator);
		break;
	case ETargetSelectionScope::InstigatorOnly:
		SelectedTargets.Add(Instigator);
		return SelectedTargets;
	default:
		break;
	}

	if (Settings.bRandomizeSelection)
	{
		Algo::RandomShuffle(CachedTargets);
	}

	if (Settings.MaxAmount > 0)
	{
		const int32 Count = FMath::Min(Settings.MaxAmount, CachedTargets.Num());
		SelectedTargets.Append(CachedTargets.GetData(), Count);
	}
	else
	{
		SelectedTargets = CachedTargets;
	}

	return SelectedTargets;
}
