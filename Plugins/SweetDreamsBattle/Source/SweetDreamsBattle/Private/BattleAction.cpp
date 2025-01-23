// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleAction.h"
#include "BattleCharacter.h"
#include "TurnBasedBattle.h"
#include "BattleState.h"
#include "SweetDreamsBPLibrary.h"

UBattleAction* UBattleAction::CreateLearnAction(AActor* ActionOwner, TSubclassOf<UBattleAction> Action)
{
	UBattleAction* NewAction = nullptr;
	if (IsValid(Action) && IsValid(ActionOwner))
	{
		UBattlerDataComponent* Data = UBattlerDataComponent::GetBattlerDataComponent(ActionOwner);
		if (IsValid(Data))
		{
			return Data->LearnAction(Action);
		}
	}
	return NewAction;
}

TArray<UBattleAction*> UBattleAction::CreateLearnActions(AActor* ActionOwner, TArray<TSubclassOf<UBattleAction>> Actions)
{
	TArray<UBattleAction*> NewActions;
	if (Actions.Num() > 0 && IsValid(ActionOwner))
	{
		UBattlerDataComponent* Data = UBattlerDataComponent::GetBattlerDataComponent(ActionOwner);
		if (IsValid(Data))
		{
			return Data->LearnActions(Actions);
		}
	}
	return NewActions;
}

void UBattleAction::StartAction(bool bUseCooldown)
{
	if (IsValid(GetOwner()))
	{
		UBattlerDataComponent* BattleParams = UBattlerDataComponent::GetBattlerDataComponent(GetOwner());
		if (IsValid(BattleParams))
		{
			BattleParams->IncrementActionCount();
			if (BattleParams->IsDead() || !BattleParams->GetIsAbleToAct())
			{
				if (bTurnBasedAction)
				{
					StartNextTurnAction(0.1f);
				}
				return;
			}
		}
	}
	if (!IsValid(GetOwner()) || bSkipThis)
	{
		if (bTurnBasedAction)
		{
			StartNextTurnAction(0.1f);
		}
		return;
	}
	if (bAutoConsumeCost)
	{
		ApplyConsumeCost();
	}
	if (IsValid(StartingSequence))
	{
		float SequenceTime = PlayLevelSequence(StartingSequence);
		if (bWaitSequenceToStart && SequenceTime > 0.f)
		{
			FTimerHandle LocalTimer;
			GetOwner()->GetWorldTimerManager().SetTimer(LocalTimer, [this]() {
				OnActionStart();
				}, SequenceTime, false);
			return;
		}
	}
	ShowMessage();
	OnActionStart();
}

void UBattleAction::StartActionForced(bool bUseCooldown)
{
	if (!IsValid(CurrentBattle))
	{
		int32 BattleId = -1;
		CurrentBattle = ASweetDreamsBattleManager::FindActiveBattle(Owner, BattleId);
	}
	if (IsValid(CurrentBattle) && bTurnBasedAction)
	{
		ATurnBasedBattle* TurnBattle = Cast<ATurnBasedBattle>(CurrentBattle);
		if (IsValid(TurnBattle))
		{
			bSkipThis = false;
			UBattleAction* ThisAction = this;
			TurnBattle->GetTargetsAllPossible(ThisAction, false);
			SetTargetRandom(ElementTargets, TargetAmount);
			if (bUseCooldown)
			{
				RefreshCooldown();
			}
			TurnBattle->AddTurnAction(this, bAddedLast);
			return;
		}
	}
	StartAction(bUseCooldown);
}

void UBattleAction::RefreshCooldown()
{
	if (!IsValid(Owner)) return;
	bIsOnCooldown = true;
	TurnsPassed = -1;
	if (!bTurnBasedAction)
	{
		Owner->GetWorldTimerManager().SetTimer(ActionCooldown, this, &UBattleAction::UpdateCooldown, Cooldown, false);
	}
	else
	{
		if (CooldownTurns <= 0)
		{
			bIsOnCooldown = false;
			return;
		}
		UpdateCooldown();
	}
}

void UBattleAction::UpdateCooldown()
{
	if (bIsOnCooldown)
	{
		if (!bTurnBasedAction)
		{
			bIsOnCooldown = false;
		}
		else
		{
			TurnsPassed = FMath::Clamp(TurnsPassed + 1, 0, CooldownTurns);
			if (TurnsPassed >= CooldownTurns)
			{
				bIsOnCooldown = false;
			}
		}
	}
}

float UBattleAction::GetPriorityWeight() const
{
	return PriorityWeigth;
}

void UBattleAction::RemoveSelfBattle()
{
	if (bTurnBasedAction && CurrentBattle)
	{
		ATurnBasedBattle* TurnBattle = Cast<ATurnBasedBattle>(CurrentBattle);
		if (TurnBattle)
		{
			bSkipThis = true;
			TurnBattle->RemoveTurnAction(this);
		}
	}
}

void UBattleAction::EndAction(float Delay)
{
	AActor* ParamOwner;
	UBattlerDataComponent* BattleParams;
	if (bGetManaFromBattleManager)
	{
		if (!IsValid(CurrentBattle))
		{
			int32 Temp = 1;
			CurrentBattle = ASweetDreamsBattleManager::FindActiveBattle(this, Temp);
		}
		ParamOwner = CurrentBattle;
	}
	else
	{
		ParamOwner = GetOwner();
	}
	if (IsValid(ParamOwner))
	{
		BattleParams = UBattlerDataComponent::GetBattlerDataComponent(ParamOwner);
	}
	if (Delay == 0.0f)
	{
		Delay = 0.01f;
	}
	FTimerHandle LocalTimer;
	Owner->GetWorldTimerManager().SetTimer(LocalTimer, [this, BattleParams](){
		OnActionEnd();
		if (BattleParams)
		{
			if (HealAmount > 0) BattleParams->ReceiveHeal(HealAmount * (BattleParams->GetHealMultiplier() / 100));
			if (ManaRestoreAmount > 0) BattleParams->ReceiveManaRestore(ManaRestoreAmount * (BattleParams->GetManaRestoreMultiplier() / 100));
		}
	}, Delay, false);
	if (Owner)
	{
		TArray<UBattleState*> States;
		if (BattleParams)
		{
			States = BattleParams->GetAllStates();
		}
		if (States.Num() > 0)
		{
			for (UBattleState* State : States)
			{
				State->ConsumeLifetime(EStateLifetime::Action);
				State->OnActionEnd(this);
			}
		}
		Owner->GetWorldTimerManager().ClearTimer(ActionTimer);
	}
}

void UBattleAction::StartNextTurnAction(float Delay)
{
	FTimerHandle LocalTimer;
	ATurnBasedBattle* TurnBattle = nullptr;
	if (!IsValid(CurrentBattle))
	{
		int32 Index = 0;
		TurnBattle = ATurnBasedBattle::FindActiveTurnBattle(GetOwner(), Index);
	}
	else
	{
		TurnBattle = Cast<ATurnBasedBattle>(CurrentBattle);
	}
	if (IsValid(TurnBattle) && IsValid(Owner) && bTurnBasedAction)
	{
		Owner->GetWorldTimerManager().SetTimer(LocalTimer, [TurnBattle, this]() {
			RemoveSelfBattle();
			TurnBattle->StartTurnAction();
			}, Delay, false);
	}
}

void UBattleAction::ResetAction()
{
	ResetSkipAction();
	TurnsPassed = -1;
	bIsOnCooldown = false;
	RemoveSelfBattle();
	ElementTargets.Empty();
}

void UBattleAction::ResetSkipAction()
{
	bSkipThis = false;
}

bool UBattleAction::IsActionAvailable()
{
	float Mana = 0.f;
	float MaxMana = 0.f;
	int32 Temp = 0;
	AActor* ParamOwner;
	UBattlerDataComponent* BattleParams;
	if (bGetManaFromBattleManager)
	{
		if (!IsValid(CurrentBattle))
		{
			CurrentBattle = ASweetDreamsBattleManager::FindActiveBattle(this, Temp);
		}
		ParamOwner = CurrentBattle;
	}
	else
	{
		ParamOwner = GetOwner();
	}
	if (IsValid(ParamOwner))
	{
		BattleParams = UBattlerDataComponent::GetBattlerDataComponent(ParamOwner);
		if (BattleParams)
		{
			Mana = BattleParams->GetMana(MaxMana);
		}
	}
	if (Cost > Mana || bIsOnCooldown)
	{
		return false;
	}
	return OnAvailabilityRule();
}

ETargetType UBattleAction::GetTargetType() const
{
	return TargetType;
}

int32 UBattleAction::GetTargetAmount() const
{
	return TargetAmount;
}

bool UBattleAction::GetIfIncludeSelf() const
{
	return bIncludeSelf;
}

int32 UBattleAction::GetActionSpeed() const
{
	float Speed = 0.f;
	if (GetBattleOwner())
	{
		Speed = GetBattleOwner()->GetBattlerParameters()->GetSpeed();
	}
	else
	{
		UBattlerDataComponent* BattleParams = GetOwner()->FindComponentByClass<UBattlerDataComponent>();
		if (BattleParams)
		{
			Speed = BattleParams->GetSpeed();
		}
	}
	if (bOverrideOwnerSpeed)
	{
		return ActionSpeed;
	}
	return Speed;
}

void UBattleAction::SetActionSpeed(int32 NewSpeed)
{
	ActionSpeed = NewSpeed;
}

float UBattleAction::GetActionCost() const
{
	return Cost;
}

bool UBattleAction::ApplyConsumeCost() // this function consumes its cost independently if it was available to use or not
{
	int32 Temp;
	UBattlerDataComponent* BattleParams = nullptr;
	ASweetDreamsBattleManager* Battle = ASweetDreamsBattleManager::FindActiveBattle(this, Temp);
	if (bGetManaFromBattleManager && Battle && UBattlerDataComponent::GetBattlerDataComponent(Battle))
	{
		BattleParams = UBattlerDataComponent::GetBattlerDataComponent(Battle);
	}
	else
	{
		BattleParams = UBattlerDataComponent::GetBattlerDataComponent(GetOwner());
	}
	if (BattleParams)
	{
		BattleParams->ReceiveManaConsume(GetActionCost());
		return true;
	}
	return false;
}

bool UBattleAction::DamageTargets(TArray<AActor*> Targets, float& PostMitigatedDamage, int32& KilledTargets, float Damage, float ResistenceShred, bool bCanBeMitigated, bool bApplyCalculations)
{
	bool bTargetsKilled = Super::DamageTargets(Targets, PostMitigatedDamage, KilledTargets, Damage, ResistenceShred, bCanBeMitigated, bApplyCalculations);
	AActor* ParamOwner;
	if (bGetManaFromBattleManager)
	{
		if (!IsValid(CurrentBattle))
		{
			int32 Temp = 1;
			CurrentBattle = ASweetDreamsBattleManager::FindActiveBattle(this, Temp);
		}
		ParamOwner = CurrentBattle;
	}
	else
	{
		ParamOwner = GetOwner();
	}
	if (IsValid(ParamOwner))
	{
		UBattlerDataComponent* BattleParams = UBattlerDataComponent::GetBattlerDataComponent(ParamOwner);
		if (BattleParams)
		{
			if (HealthVampirism > 0.f) BattleParams->ReceiveHeal(PostMitigatedDamage * (HealthVampirism / 100.f));
			if (ManaVampirism > 0.f) BattleParams->ReceiveManaRestore(PostMitigatedDamage * (ManaVampirism / 100.f));
		}
	}
	return bTargetsKilled;
}

float UBattleAction::StartAnimation(UAnimSequence* Animation, TArray<AActor*> Targets)
{
	float AnimationTime = Super::StartAnimation(Animation, Targets);
	FTimerHandle AnimationTimer;
	Owner->GetWorldTimerManager().SetTimer(AnimationTimer, [Animation, this] {
		OnAnimationComplete(Animation);
		}, AnimationTime, false);
	return AnimationTime;
}

void UBattleAction::MoveToTarget(AActor* Target, int32 MovementID)
{
	if (!Owner || !Target) return;
	APawn* PawnOwner = Cast<APawn>(GetOwner());
	if (PawnOwner)
	{
		ASweetDreamsController* BattlerController = Cast<ASweetDreamsController>(PawnOwner->GetController());
		if (BattlerController)
		{
			BattlerController->SetCurrentMovementID(MovementID);
			BattlerController->OnDreamMovementCompleted.AddUniqueDynamic(this, &UBattleAction::OnMovementComplete);
			BattlerController->MoveToActor(Target);
		}
	}
}

void UBattleAction::ReturnToPosition(float Delay)
{
	if (!Owner) return;
	APawn* PawnOwner = Cast<APawn>(GetOwner());
	if (PawnOwner)
	{
		ASweetDreamsController* BattlerController = Cast<ASweetDreamsController>(PawnOwner->GetController());
		if (BattlerController)
		{
			if (Delay <= 0.0f) Delay = GetOwner()->GetWorld()->GetDeltaSeconds();
			FTimerHandle ReturnTimer;
			Owner->GetWorldTimerManager().SetTimer(ReturnTimer, [this, BattlerController] {
				BattlerController->ResetPosition();
				}, Delay, false);
		}
	}
}

float UBattleAction::PlayLevelSequence(ULevelSequence* Sequence)
{
	if (!IsValid(Sequence) || !IsValid(CurrentBattle)) return 0.f;
	ULevelSequencePlayer* SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), Sequence, FMovieSceneSequencePlaybackSettings(), StartingSequenceActor);
	float Time = 0.f;
	if (IsValid(SequencePlayer))
	{
		Time = SequencePlayer->GetDuration().AsSeconds();
		OnSequenceBindings(SequencePlayer);
		SequencePlayer->Play();
		FTimerHandle SequenceTimer;
		Owner->GetWorldTimerManager().SetTimer(SequenceTimer, [Sequence, this]() {
			OnSequenceComplete(Sequence);
			}, Time, false);
	}
	return Time;
}
