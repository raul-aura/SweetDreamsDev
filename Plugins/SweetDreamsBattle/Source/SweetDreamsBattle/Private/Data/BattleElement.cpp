// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleElement.h"
#include "BattleCharacter.h"
#include "SweetDreamsBattleManager.h"
#include "Kismet/GameplayStatics.h"
#include "Core/SweetDreamsBPLibrary.h"
#include "BattleState.h"
#include "TurnBasedBattle.h"
#include "Dialogue/SweetDreamsDialogueManager.h"
#include "Algo/AllOf.h"
#include "BattlerDataComponent.h"

void UBattleElement::SetOwner(AActor* InputOwner)
{
	Owner = InputOwner;
}

AActor* UBattleElement::GetOwner() const
{
	return Owner;
}

ABattleCharacter* UBattleElement::GetBattleOwner() const
{
	return Cast<ABattleCharacter>(GetOwner());
}

TArray<AActor*> UBattleElement::GetOwnerAsArray() const
{
	TArray<AActor*> ArrayOwner;
	ArrayOwner.Add(GetOwner());
	return ArrayOwner;
}

void UBattleElement::ShowMessage()
{
	if (CurrentBattle)
	{
		ATurnBasedBattle* TurnBattle = Cast<ATurnBasedBattle>(CurrentBattle);
		if (IsValid(TurnBattle))
		{
			UTurnBasedBattleWidget* Widget = TurnBattle->GetTurnBattleWidget();
			if (IsValid(Widget))
			{
				Widget->OnElementMessage(ElementMessage);
			}
		}
	}
}

void UBattleElement::UpdateElementDescription(FText NewDescription)
{
	ElementDescription = NewDescription;
}

void UBattleElement::SetElementHidden(bool bIsHidden)
{
	bIsElementHidden = bIsHidden;
}

void UBattleElement::SetCurrentBattle(ASweetDreamsBattleManager* Battle)
{
	CurrentBattle = Battle;
}

void UBattleElement::AddTarget(AActor* Target, bool bRemoveDead)
{
	if (!ElementTargets.Contains(Target))
	{
		ElementTargets.Add(Target);
	}
	if (bRemoveDead)
	{
		UpdateValidTargets();
	}
}

void UBattleElement::RemoveTarget(AActor* Target)
{
	if (ElementTargets.Contains(Target))
	{
		ElementTargets.Remove(Target);
	}
}

void UBattleElement::SetTarget(TArray<AActor*> NewTargets, bool bRemoveDead)
{
	ElementTargets.Empty();
	ElementTargets = NewTargets;
	if (bRemoveDead)
	{
		UpdateValidTargets();
	}
}

void UBattleElement::SetTargetRandom(TArray<AActor*> PossibleTargets, int32 TargetAmount, bool bRemoveDead)
{
	if (PossibleTargets.Num() == 0 || TargetAmount <= 0) return;
	ElementTargets.Empty();
	if (PossibleTargets.Num() <= TargetAmount)
	{
		ElementTargets = PossibleTargets;
	}
	else
	{
		TSet<int32> SelectedIndices;
		while (SelectedIndices.Num() < TargetAmount)
		{
			int32 RandomIndex = FMath::RandRange(0, PossibleTargets.Num() - 1);
			if (!SelectedIndices.Contains(RandomIndex))
			{
				SelectedIndices.Add(RandomIndex);
				AddTarget(PossibleTargets[RandomIndex], bRemoveDead);
			}
		}
	}
	if (bRemoveDead)
	{
		UpdateValidTargets();
	}
}

TArray<AActor*> UBattleElement::GetAdjacentTargets(AActor* PrimaryTarget, const TArray<AActor*>& TargetsToSearch) const
{
	TArray<AActor*> FoundTargets;
	if (!IsValid(PrimaryTarget) || !AreTargetsValid(TargetsToSearch)) return FoundTargets;
	int32 FoundIndex = TargetsToSearch.Find(PrimaryTarget);
	if (FoundIndex != INDEX_NONE)
	{
		for (int32 Offset : {-1, 1})
		{
			int32 AdjacentIndex = FoundIndex + Offset;
			if (TargetsToSearch.IsValidIndex(AdjacentIndex))
			{
				FoundTargets.Add(TargetsToSearch[AdjacentIndex]);
			}
		}
	}
	return FoundTargets;
}

void UBattleElement::AddAdjacentTargets(AActor* PrimaryTarget, const TArray<AActor*>& TargetsToSearch)
{
	if (!IsValid(PrimaryTarget) || !AreTargetsValid(TargetsToSearch)) return;
	for (AActor* AdjacentTarget : GetAdjacentTargets(PrimaryTarget, TargetsToSearch))
	{
		if (!ElementTargets.Contains(AdjacentTarget))
		{
			ElementTargets.Add(AdjacentTarget);
		}
	}
}

AActor* UBattleElement::GetFirstElementTarget() const
{
	if (ElementTargets.Num() > 0) 
	{
		return ElementTargets[0];
	}
	return nullptr;
}

bool UBattleElement::UpdateValidTargets()
{
	if (ElementTargets.Num() == 0) return false;
	for (int32 i = ElementTargets.Num() - 1; i >= 0; --i)
	{
		AActor* Target = ElementTargets[i];
		if (!IsValid(Target))
		{
			ElementTargets.RemoveAt(i);
		}
	}
	return ElementTargets.Num() > 0;
}

bool UBattleElement::DamageTargets(TArray<AActor*> Targets, float& PostMitigatedDamage, int32& KilledTargets, float Damage, float ResistenceShred, bool bCanBeMitigated, bool bApplyCalculations, bool bIsAdditionalDamage)
{
	PostMitigatedDamage = 0.0f;
	KilledTargets = 0;
	if (!AreTargetsValid(Targets) || !IsValid(GetOwner())) return false;
	UBattlerDataComponent* OwnerData = UBattlerDataComponent::GetBattlerDataComponent(GetOwner());
	if (IsValid(OwnerData))
	{
		Damage *= (OwnerData->GetDamageDealtMultiplier() / 100.f);
		Damage = FMath::Max(Damage, 0.f);
	}
	bool bAllDead = true;
	for (AActor* Target : Targets)
	{
		if (IsValid(Target))
		{
			UBattlerDataComponent* Data;
			Data = Target->FindComponentByClass<UBattlerDataComponent>();
			if (Data)
			{
				bool bTargetDead = true;
				if (Data->IsDead())
				{
					continue;
				}
				float SinglePostMitigated = Data->ReceiveDamage(Damage, ResistenceShred, bCanBeMitigated, GetOwner(), bIsAdditionalDamage);
				PostMitigatedDamage += SinglePostMitigated;
				if (!Data->IsDead())
				{
					bTargetDead = false;
					bAllDead = false;
				}
				else
				{
					KilledTargets++;
				}
				TArray<UBattleElement*> AllElements = OwnerData->GetAllElements();
				for (UBattleElement* Element : AllElements)
				{
					Element->OnDamageDealt(Targets, SinglePostMitigated, bTargetDead);
				}
				int32 Index = 1;
				ASweetDreamsBattleManager* Battle = ASweetDreamsBattleManager::FindActiveBattle(GetOwner(), Index);
				if (Battle && Cast<ABattleCharacter>(Target))
				{
					Battle->AddDamageToBattle(Cast<ABattleCharacter>(GetOwner()), SinglePostMitigated, bApplyCalculations);
				}

			}
		}
	}
	return bAllDead;
}

void UBattleElement::HealTargets(TArray<AActor*> Targets, float& HealedAmount, float& OverhealAmount, float Heal)
{
	HealedAmount = 0.0f;
	OverhealAmount = 0.0f;
	if (!AreTargetsValid(Targets) || !IsValid(GetOwner())) return;
	UBattlerDataComponent* OwnerData = UBattlerDataComponent::GetBattlerDataComponent(GetOwner());
	if (IsValid(OwnerData))
	{
		Heal *= (OwnerData->GetHealMultiplier() / 100.f);
		Heal = FMath::Max(Heal, 0.f);
	}
	for (AActor* Target : Targets)
	{
		if (IsValid(Target))
		{
			UBattlerDataComponent* Data = UBattlerDataComponent::GetBattlerDataComponent(Target);
			if (IsValid(Data))
			{
				if (Data->IsDead())
				{
					continue;
				}
				HealedAmount += Data->ReceiveHeal(Heal);
				OverhealAmount += Heal - HealedAmount;
				TArray<UBattleElement*> AllElements = OwnerData->GetAllElements();
				for (UBattleElement* Element : AllElements)
				{
					Element->OnHealingGranted(Targets, HealedAmount, OverhealAmount);
				}
			}
		}
	}
}

void UBattleElement::RestoreManaTargets(TArray<AActor*> Targets, float& RestoredAmount, float& OverflowAmount, float Restore)
{
	RestoredAmount = 0.0f;
	OverflowAmount = 0.0f;
	if (!AreTargetsValid(Targets) || !IsValid(GetOwner())) return;
	UBattlerDataComponent* OwnerData = UBattlerDataComponent::GetBattlerDataComponent(GetOwner());
	if (IsValid(OwnerData))
	{
		Restore *= (OwnerData->GetManaRestoreMultiplier() / 100.f);
		Restore = FMath::Max(Restore, 0.f);
	}
	for (AActor* Target : Targets)
	{
		if (IsValid(Target))
		{
			UBattlerDataComponent* Data = Target->FindComponentByClass<UBattlerDataComponent>();
			if (Data)
			{
				if (Data->IsDead() || Target->IsPendingKill())
				{
					continue;
				}
				RestoredAmount += Data->ReceiveHeal(Restore);
				OverflowAmount += Restore - RestoredAmount;
				TArray<UBattleElement*> AllElements = OwnerData->GetAllElements();
				for (UBattleElement* Element : AllElements)
				{
					Element->OnManaRestoreGranted(Targets, RestoredAmount, OverflowAmount);
				}
			}
		}
	}
}

bool UBattleElement::AddStatesToTargets(UObject* StateInstigator, TArray<TSubclassOf<UBattleState>> States, TArray<AActor*> Targets, int32& StatesAdded, float Chance)
{
	if (!AreTargetsValid(Targets) || !StateInstigator || States.Num() == 0) return false;
	Chance = FMath::Clamp(Chance, 0.0f, 1.0f);
	float RandomNum = 0.0f;
	bool bAllStatesApplied = true;
	for (AActor* Target : Targets)
	{
		if (IsValid(Target))
		{
			UBattlerDataComponent* Data;
			Data = Target->FindComponentByClass<UBattlerDataComponent>();
			if (Data)
			{
				if (Data->IsDead() || Target->IsPendingKill())
				{
					continue;
				}
				RandomNum = FMath::FRand();
				if (Chance < RandomNum)
				{
					bAllStatesApplied = false;
					continue;
				}
				for (TSubclassOf<UBattleState> State : States)
				{
					Data->AddState(State, StateInstigator);
					StatesAdded++;
				}
			}
		}
	}
	return bAllStatesApplied;
}

bool UBattleElement::RemoveStatesOfTargets(TArray<TSubclassOf<UBattleState>> States, TArray<AActor*> Targets, int32& StatesRemoved, float Chance)
{
	if (!AreTargetsValid(Targets) || States.Num() == 0) return false;
	Chance = FMath::Clamp(Chance, 0.0f, 1.0f);
	float RandomNum = 0.0f;
	bool bAllStatesRemoved = true;
	for (AActor* Target : Targets)
	{
		if (IsValid(Target))
		{
			UBattlerDataComponent* Data;
			Data = Target->FindComponentByClass<UBattlerDataComponent>();
			if (Data)
			{
				if (Data->IsDead() || Target->IsPendingKill())
				{
					continue;
				}
				RandomNum = FMath::FRand();
				if (Chance < RandomNum)
				{
					bAllStatesRemoved = false;
					continue;
				}
				for (TSubclassOf<UBattleState> State : States)
				{
					Data->RemoveState(State);
					StatesRemoved++;
				}
			}
		}
	}
	return bAllStatesRemoved;
}

void UBattleElement::CleanseTargets(TArray<AActor*> Targets, int32& StatesRemoved)
{
	if (!AreTargetsValid(Targets)) return;
	for (AActor* Target : Targets)
	{
		if (IsValid(Target))
		{
			UBattlerDataComponent* Data;
			Data = Target->FindComponentByClass<UBattlerDataComponent>();
			if (Data)
			{
				if (Data->IsDead() || Target->IsPendingKill())
				{
					continue;
				}
				StatesRemoved = Data->RemoveAllStates();
			}
		}
	}
}

bool UBattleElement::DoesTargetsHasStates(TArray<AActor*> Targets, TArray<TSubclassOf<UBattleState>> States, EStateMatchCondition MatchCondition)
{
	if (!AreTargetsValid(Targets)) return false;
	for (AActor* Target : Targets)
	{
		if (IsValid(Target))
		{
			if (UBattlerDataComponent* Data = UBattlerDataComponent::GetBattlerDataComponent(Target))
			{
				TArray<UBattleState*> AllStates = Data->GetAllStates();
				if (MatchCondition == EStateMatchCondition::AnyMatch)
				{
					for (UBattleState* State : AllStates)
					{
						if (IsValid(State) && States.ContainsByPredicate([&](TSubclassOf<UBattleState> StateClass) { return State->IsA(StateClass); }))
						{
							return true;
						}
					}
				}
				else if (MatchCondition == EStateMatchCondition::AllMatch)
				{
					return Algo::AllOf(States, [&](TSubclassOf<UBattleState> StateClass)
					{
						return AllStates.ContainsByPredicate([&](UBattleState* State) { return IsValid(State) && State->IsA(StateClass); });
					});
				}
			}
		}
	}
	return false;
}

void UBattleElement::KillTargets(TArray<AActor*> Targets)
{
	if (!AreTargetsValid(Targets)) return;
	for (AActor* Target : Targets)
	{
		if (IsValid(Target))
		{
			UBattlerDataComponent* Data;
			Data = Target->FindComponentByClass<UBattlerDataComponent>();
			if (Data)
			{
				if (Data->IsDead() || Target->IsPendingKill())
				{
					continue;
				}
				Data->Kill(GetOwner());
			}
		}
	}
}

void UBattleElement::ReviveTargets(TArray<AActor*> Targets, float HealthRestore, float ManaRestore)
{
	if (!AreTargetsValid(Targets)) return;
	for (AActor* Target : Targets)
	{
		if (IsValid(Target))
		{
			UBattlerDataComponent* Data;
			Data = Target->FindComponentByClass<UBattlerDataComponent>();
			if (Data)
			{
				Data->Revive(HealthRestore, ManaRestore);
			}
		}
	}
}

void UBattleElement::SetAbilityToAct(TArray<AActor*> Targets, bool bIsAbleToAct)
{
	if (!AreTargetsValid(Targets)) return;
	for (AActor* Target : Targets)
	{
		if (IsValid(Target))
		{
			UBattlerDataComponent* Data = UBattlerDataComponent::GetBattlerDataComponent(Target);
			if (Data)
			{
				Data->SetIsAbleToAct(bIsAbleToAct);
			}
		}
	}
}

float UBattleElement::StartAnimation(UAnimSequence* Animation, TArray<AActor*> Targets)
{
	if (!AreTargetsValid(Targets) || !Animation) return 0.f;
	for (AActor* Target : Targets)
	{
		if (IsValid(Target))
		{
			if (USkeletalMeshComponent* Mesh = Target->FindComponentByClass<USkeletalMeshComponent>())
			{
				if (Mesh->GetAnimationMode() == EAnimationMode::AnimationBlueprint)
				{
					Mesh->SetAnimationMode(EAnimationMode::AnimationSingleNode);
					float PlayLength = Animation->GetPlayLength();
					FTimerHandle AnimTimer;
					GetWorld()->GetTimerManager().SetTimer(AnimTimer, [Mesh]()
						{
							Mesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
						}, PlayLength, false);
				}
				Mesh->PlayAnimation(Animation, false);
			}
		}
	}
	return Animation->GetPlayLength();
}

void UBattleElement::CreateAndForceAction(TSubclassOf<UBattleAction> Action, TArray<AActor*> Targets, bool bUseCooldown, int32 OverrideSpeed)
{
	if (!AreTargetsValid(Targets) || !IsValid(Action)) return;
	for (AActor* Target : Targets)
	{
		if (IsValid(Target))
		{
			UBattlerDataComponent* Data = UBattlerDataComponent::GetBattlerDataComponent(Target);
			if (IsValid(Data))
			{
				UBattleAction* NewAction = NewObject<UBattleAction>(Target, Action);
				if (IsValid(NewAction))
				{
					NewAction->SetOwner(Target);
					if (OverrideSpeed > 0)
					{
						NewAction->SetActionSpeed(OverrideSpeed);
					}
					NewAction->StartActionForced(bUseCooldown);
				}
			}
		}
	}
}

void UBattleElement::TriggerSound(USoundBase* Sound, float Volume, float Pitch, float Delay)
{
	if (Sound)
	{
		UGameplayStatics::PlaySound2D(Owner, Sound, Volume, Pitch, Delay);
	}
}

void UBattleElement::TriggerSoundAtLocation(USoundBase* Sound, FVector Location, float Volume, float Pitch, float Delay)
{
	if (Sound)
	{
		UGameplayStatics::PlaySoundAtLocation(Owner, Sound, Location, Volume, Pitch, Delay);
	}
}

ASweetDreamsDialogueManager* UBattleElement::StartDialogue(FName DialogueName, float StartTransition)
{
	if (!DialogueName.IsNone())
	{
		ASweetDreamsDialogueManager* FoundDialogue = ASweetDreamsDialogueManager::StartDialogueByName(GetOwner(), DialogueName, StartTransition);
		if (IsValid(FoundDialogue))
		{
			FoundDialogue->OnDialogueEnded.AddDynamic(this, &UBattleElement::OnDialogueEnded);
			OnDialogueStarted();
			return FoundDialogue;
		}
	}
	return nullptr;
}

bool UBattleElement::AreTargetsValid(const TArray<AActor*>& Targets)
{
	return (Targets.Num() > 0);
}

float UBattleElement::OnPreDamageReceived_Implementation(float DamageAmount, AActor* DamageInstigator)
{
	return DamageAmount;
}
