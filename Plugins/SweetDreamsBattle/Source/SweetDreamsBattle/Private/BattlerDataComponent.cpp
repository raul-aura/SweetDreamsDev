// Fill out your copyright notice in the Description page of Project Settings.

#include "BattlerDataComponent.h"
#include "BattleState.h"
#include "BattleCharacter.h"
#include "SweetDreamsBPLibrary.h"

UBattlerDataComponent::UBattlerDataComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UBattlerDataComponent::BeginPlay()
{
	MaxHealth = BaseHealth;
	Health = MaxHealth;
	MaxMana = BaseMana;
	Mana = bOverrideStartingMana ? StartingMana : MaxMana;
	CurrentLives = FMath::Max(1, AdditionalLives + 1);
	Force = BaseForce;
	Resistence = BaseResistence;
	Speed = BaseSpeed;
	if (IsValid(LevelClass))
	{
		LevelObject = NewObject<USweetDreamsLevel>(this, LevelClass);
		if (IsValid(LevelObject))
		{
			LevelObject->OnLevelUpgraded.AddUniqueDynamic(this, &UBattlerDataComponent::UpdateParametersByLevel);
		}
	}
	CreateActions();
	Super::BeginPlay();
}

void UBattlerDataComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	for (UBattleState* State : AllStates)
	{
		if (IsValid(State)) State->ConsumeLifetime(EStateLifetime::Second);
	}
	for (UBattleElement* Element : AllElements)
	{
		if (IsValid(Element)) Element->OnOwnerTick(DeltaTime);
	}
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

ABattleCharacter* UBattlerDataComponent::GetBattlerOwner() const
{
	return Cast<ABattleCharacter>(GetOwner());
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

void UBattlerDataComponent::UpdateParametersByLevel(int32 NewLevel)
{
	// do stuff here later
}

float UBattlerDataComponent::GetHealth(float& MaximumHealth, float Multiplier)
{
	MaximumHealth = MaxHealth * (Multiplier / 100);
	return Health * (Multiplier / 100);
}

float UBattlerDataComponent::GetMana(float& MaximumMana, float Multiplier)
{
	MaximumMana = MaxMana * (Multiplier / 100);
	return Mana * (Multiplier / 100);
}

float UBattlerDataComponent::GetForce(float Multiplier) const
{
	return Force * (Multiplier / 100) * (GetForceMultiplier() / 100);
}

float UBattlerDataComponent::GetResistence(float Multiplier) const
{
	return Resistence * (Multiplier / 100) * (GetResistenceMultiplier() / 100);
}

int32 UBattlerDataComponent::GetSpeed(float Multiplier) const
{
	return Speed * (Multiplier / 100) * (GetSpeedMultiplier() / 100);
}

int32 UBattlerDataComponent::GetAdditionalActions() const
{
	return AdditionalActions;
}

int32 UBattlerDataComponent::GetBaseActionsPerTurn() const
{
	return BaseActionsPerTurn;
}

int32 UBattlerDataComponent::GetActionsPerTurn() const
{
	return GetBaseActionsPerTurn() + GetAdditionalActions();
}

void UBattlerDataComponent::SetIsAbleToAct(bool bNewAbility)
{
	bIsAbleToAct = bNewAbility;
}

float UBattlerDataComponent::ReceiveDamage(float Damage, float ResistenceShred, bool bCanBeMitigated, AActor* DamageInstigator, bool bIsAdditionalDamage)
{
	if (Damage < 0) return 0.f;
	float FinalDamage = Damage;
	for (UBattleElement* Element : AllElements)
	{
		if (IsValid(Element)) FinalDamage = Element->OnPreDamageReceived(FinalDamage, DamageInstigator);
	}
	if (bCanBeMitigated)
	{
		FinalDamage = OnMitigateDamage(FinalDamage, ResistenceShred);
	}
	FinalDamage = FMath::Clamp(FinalDamage * (GetDamageReceivedMultiplier() / 100.f), 0.f, Health);
	if (GetBattlerOwner())
	{
		if (bCanBeMitigated)
		{
			FinalDamage = GetBattlerOwner()->OnCharacterMitigateDamage(FinalDamage);
			FinalDamage = FMath::Clamp(FinalDamage, 0.f, Health);
		}
		GetBattlerOwner()->OnDamageReceived(FinalDamage);
		GetBattlerOwner()->IndicateDamage(FinalDamage);
	}
	Health = FMath::Clamp(Health - FinalDamage, 0.f, MaxHealth);
	for (UBattleElement* Element : AllElements)
	{
		if (IsValid(Element)) Element->OnPostDamageReceived(FinalDamage, DamageInstigator, bIsAdditionalDamage);
	}
	if (Health <= 0)
	{
		Kill(DamageInstigator);
	}
	return FinalDamage;
}

float UBattlerDataComponent::ReceiveHeal(float Heal)
{
	if (Heal < 0.f) return 0.f;
	float HealedAmount = FMath::Min(Heal, (MaxHealth - Health));
	Health = FMath::Clamp(Health + Heal, 0.f, MaxHealth);
	if (GetBattlerOwner())
	{
		GetBattlerOwner()->IndicateDamage(HealedAmount, true);
	}
	for (UBattleElement* Element : AllElements)
	{
		if (IsValid(Element)) Element->OnHealed(HealedAmount);
	}
	return HealedAmount;
}

float UBattlerDataComponent::ReceiveManaConsume(float Consume)
{
	if (Consume <= 0) return 0.f;
	float ConsumedAmount = FMath::Min(Consume, Mana);
	Mana = FMath::Clamp(Mana - Consume, 0.f, MaxMana);
	for (UBattleElement* Element : AllElements)
	{
		if (IsValid(Element)) Element->OnManaConsumed(ConsumedAmount);
	}
	return ConsumedAmount;
}

float UBattlerDataComponent::ReceiveManaRestore(float Restore)
{
	if (Restore <= 0) return 0.f;
	float RestoredAmount = FMath::Min(Restore, (MaxMana - Mana));
	Mana = FMath::Clamp(Mana + Restore, 0.f, MaxMana);
	for (UBattleElement* Element : AllElements)
	{
		if (IsValid(Element)) Element->OnManaRestored(RestoredAmount);
	}
	return RestoredAmount;
}

float UBattlerDataComponent::OnMitigateDamage_Implementation(float Damage, float ResistenceShred)
{
	float Res = GetResistence(50.f);
	float Shred = (100.f - ResistenceShred) / 100.f;
	return Damage - (Res * Shred);
}

void UBattlerDataComponent::Kill(AActor* KillInstigator)
{
	bIsDead = true;
	CurrentLives = FMath::Clamp(--CurrentLives, 0, AdditionalLives);
	GetBattlerOwner()->OnKilled(CurrentLives);
	if (CurrentLives > 0)
	{
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
			{
				Revive();
			}, 0.5f, false);
		return;
	}
	for (UBattleElement* Element : AllElements)
	{
		if (IsValid(Element)) Element->OnKilled(KillInstigator, CurrentLives);
	}
	ResetActions();
}

void UBattlerDataComponent::Revive(float HealthRestore, float ManaRestore)
{
	if (IsDead())
	{
		bIsDead = false;
		Health = MaxHealth * (HealthRestore / 100);
		Mana = MaxMana * (ManaRestore / 100);
	}
	GetBattlerOwner()->OnRevived();
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
		for (UBattleElement* Element : AllElements)
		{
			if (IsValid(Element))
			{
				Element->SetCurrentBattle(BattleReference);
			}
		}
	}
}

TArray<UBattleElement*> UBattlerDataComponent::GetAllElements() const
{
	return AllElements;
}

float UBattlerDataComponent::UpdateHealthValue(float Value, float Percentage, bool bUpdateCurrentHealth)
{
	float OldMaxHealth = MaxHealth;
	float CurrentPercentage = GetHealthPercentage();
	UpdateParameter(MaxHealth, Value, Percentage);
	if (bUpdateCurrentHealth && OldMaxHealth != MaxHealth)
	{
		Health = FMath::Clamp(MaxHealth * CurrentPercentage, 0.f, MaxHealth);
	}
	return MaxHealth;
}

float UBattlerDataComponent::UpdateManaValue(float Value, float Percentage, bool bUpdateCurrentMana)
{
	float OldMaxMana = MaxMana;
	float CurrentPercentage = GetManaPercentage();
	UpdateParameter(MaxMana, Value, Percentage);
	if (bUpdateCurrentMana && OldMaxMana != MaxMana)
	{
		Mana = FMath::Clamp(MaxMana * CurrentPercentage, 0.f, MaxMana);
	}
	return MaxMana;
}

float UBattlerDataComponent::GetHealthPercentage() const
{
	return Health / MaxHealth;
}

float UBattlerDataComponent::GetManaPercentage() const
{
	return Mana / MaxMana;
}

void UBattlerDataComponent::AddState(TSubclassOf<UBattleState> State, UObject* StateInstigator)
{
	if (!IsValid(State) || !StateInstigator) return;
	UBattleState* NewState = nullptr;
	for (UBattleState* ForState : AllStates)
	{
		if (IsValid(ForState) && ForState->IsA(State))
		{
			NewState = ForState;
			break;
		}
	}
	if (!IsValid(NewState))
	{
		NewState = NewObject<UBattleState>(GetOwner(), State);
		NewState->SetOwner(GetOwner());
		AllElements.Add(NewState);
		AllStates.Add(NewState);
	}
	NewState->ApplyState(StateInstigator);
}

void UBattlerDataComponent::AddStates(TArray<TSubclassOf<UBattleState>> StatesToAdd, UObject* StateInstigator)
{
	if (StatesToAdd.Num() == 0 || !IsValid(StateInstigator)) return;
	for (TSubclassOf<UBattleState> StateClass : StatesToAdd)
	{
		UBattleState* NewState = nullptr;
		for (UBattleState* ForState : AllStates)
		{
			if (IsValid(ForState) && ForState->IsA(StateClass))
			{
				NewState = ForState;
				break;
			}
		}
		if (!IsValid(NewState))
		{
			NewState = NewObject<UBattleState>(GetOwner(), StateClass);
			NewState->SetOwner(GetOwner());
			AllElements.Add(NewState);
			AllStates.Add(NewState);
		}
		NewState->ApplyState(StateInstigator);
	}
}

void UBattlerDataComponent::RemoveState(TSubclassOf<UBattleState> State)
{
	if (!IsValid(State) || AllStates.Num() == 0) return;
	for (UBattleState* LocalState : AllStates)
	{
		if (IsValid(LocalState) && LocalState->IsA(State))
		{
			LocalState->OnRemoved();
			LocalState->RemoveParams();
			AllElements.Remove(LocalState);
			AllStates.Remove(LocalState);
			LocalState->ConditionalBeginDestroy();
			return;
		}
	}
}

void UBattlerDataComponent::RemoveStates(TArray<TSubclassOf<UBattleState>> StatesToRemove)
{
	if (StatesToRemove.Num() == 0 || AllStates.Num() == 0) return;
	for (TSubclassOf<UBattleState> StateClass : StatesToRemove)
	{
		for (UBattleState* LocalState : AllStates)
		{
			if (IsValid(LocalState) && LocalState->IsA(StateClass))
			{
				LocalState->OnRemoved();
				LocalState->RemoveParams();
				AllElements.Remove(LocalState);
				AllStates.Remove(LocalState);
				LocalState->ConditionalBeginDestroy();
				break;
			}
		}
	}
}

int32 UBattlerDataComponent::RemoveAllStates(bool bIncludePositive, bool bIncludeNegative)
{
	if (AllStates.Num() == 0) return 0;
	int32 StatesRemoved = 0;
	for (int32 i = AllStates.Num() - 1; i >= 0; i--)
	{
		UBattleState* State = AllStates[i];
		if (!IsValid(State)) continue;
		EStateType StateType = State->GetStateType();
		if ((StateType == EStateType::POSITIVE && bIncludePositive) || (StateType == EStateType::NEGATIVE && bIncludeNegative))
		{
			State->OnRemoved();
			State->RemoveParams();
			AllStates.RemoveAt(i);
			State->ConditionalBeginDestroy();
			StatesRemoved++;
		}
	}
	return StatesRemoved;
}

UBattleState* UBattlerDataComponent::GetStateOfClass(TSubclassOf<UBattleState> StateClass) const
{
	UBattleState* FoundState = nullptr;
	for (UBattleState* State : AllStates)
	{
		if (IsValid(State) && State->IsA(StateClass))
		{
			FoundState = State;
			break;
		}
	}
	return FoundState;
}

TArray<UBattleState*> UBattlerDataComponent::GetAllStates() const
{
	return AllStates;
}

void UBattlerDataComponent::CreateActions()
{
	if (ActionClasses.Num() > 0)
	{
		for (FActionData ActionData : ActionClasses)
		{
			if (IsValid(LevelObject) && LevelObject->GetCurrentLevel() >= ActionData.Level) LearnAction(ActionData.Action);
			else if (!IsValid(LevelObject)) LearnAction(ActionData.Action);
		}
	}
}

void UBattlerDataComponent::AddAction(UBattleAction* ActionToAdd)
{
	if (IsValid(ActionToAdd))
	{
		Actions.AddUnique(ActionToAdd);
		AllElements.Add(ActionToAdd);
	}
}

void UBattlerDataComponent::AddActions(TArray<UBattleAction*> ActionsToAdd)
{
	if (ActionsToAdd.Num() > 0)
	{
		for (UBattleAction* Action : ActionsToAdd)
		{
			AddAction(Action);
		}
	}
}

UBattleAction* UBattlerDataComponent::LearnAction(TSubclassOf<UBattleAction> ActionToLearn)
{
	UBattleAction* NewAction = nullptr;
	if (IsValid(ActionToLearn))
	{
		for (UBattleAction* Action : Actions)
		{
			if (Action->IsA(ActionToLearn)) return NewAction;
		}
		NewAction = NewObject<UBattleAction>(GetOwner(), ActionToLearn);
		if (IsValid(NewAction))
		{
			NewAction->SetOwner(GetOwner());
			AddAction(NewAction);
		}
	}
	return NewAction;
}

TArray<UBattleAction*> UBattlerDataComponent::LearnActions(TArray<TSubclassOf<UBattleAction>> ActionsToLearn)
{
	TArray<UBattleAction*> NewActions;
	if (ActionsToLearn.Num() > 0)
	{
		for (TSubclassOf<UBattleAction> ActionClass : ActionsToLearn)
		{
			if (IsValid(ActionClass))
			{
				UBattleAction* NewAction = LearnAction(ActionClass);
				if (IsValid(NewAction))
				{
					NewActions.Add(NewAction);
				}
			}
		}
	}
	return NewActions;
}

void UBattlerDataComponent::RemoveAction(TSubclassOf<UBattleAction> ActionToRemove)
{
	if (IsValid(ActionToRemove))
	{
		for (UBattleAction* Action : Actions)
		{
			if (IsValid(Action) && Action->IsA(ActionToRemove))
			{
				Actions.Remove(Action);
				AllElements.Remove(Action);
				Action->ConditionalBeginDestroy();
				return;
			}
		}
	}
}

void UBattlerDataComponent::RemoveActions(TArray<TSubclassOf<UBattleAction>> ActionsToRemove)
{
	if (ActionsToRemove.Num() > 0)
	{
		TArray<UBattleAction*> ActionsToDelete;
		for (UBattleAction* Action : Actions)
		{
			if (IsValid(Action))
			{
				for (TSubclassOf<UBattleAction> ActionClass : ActionsToRemove)
				{
					if (Action->IsA(ActionClass))
					{
						ActionsToDelete.Add(Action);
						break;
					}
				}
			}
		}
		for (UBattleAction* ActionToRemove : ActionsToDelete)
		{
			Actions.Remove(ActionToRemove);
			AllElements.Remove(ActionToRemove);
			ActionToRemove->ConditionalBeginDestroy();
		}
	}
}

UBattleAction* UBattlerDataComponent::GetRandomAction() const
{
	if (Actions.Num() == 0) return nullptr;
	TArray<UBattleAction*> AvailableActions;
	if (GetAllAvailableActions(AvailableActions))
	{
		TArray<float> Weights;
		float TotalWeight = 0.f;
		for (UBattleAction* Action : AvailableActions)
		{
			if (IsValid(Action))
			{
				float Weight = Action->GetPriorityWeight();
				Weights.Add(Weight);
				TotalWeight += Weight;
			}
		}
		if (TotalWeight == 0.f) return nullptr;
		for (float& Weight : Weights) Weight /= TotalWeight;
		float RandomNum = FMath::FRand();
		float AccWeight = 0.f;
		for (int32 Index = 0; Index < Weights.Num(); Index++)
		{
			AccWeight += Weights[Index];
			if (AccWeight >= RandomNum) return AvailableActions[Index];
		}
	}
	return nullptr;
}

bool UBattlerDataComponent::GetAllAvailableActions(TArray<UBattleAction*>& AvailableActions) const
{
	AvailableActions.Empty();
	for (UBattleAction* Action : Actions)
	{
		if (Action->IsActionAvailable())
		{
			AvailableActions.Add(Action);
		}
	}
	return AvailableActions.Num() > 0;
}

TArray<UBattleAction*> UBattlerDataComponent::GetAllActions() const
{
	return Actions;
}

void UBattlerDataComponent::UpdateTurnActionsCooldown()
{
	if (Actions.Num() == 0) return;
	for (UBattleAction* Action : Actions)
	{
		Action->UpdateTurnCooldown();
	}
}

void UBattlerDataComponent::ResetActions()
{
	if (Actions.Num() == 0) return;
	for (UBattleAction* Action : Actions)
	{
		Action->ResetAction();
	}
}

void UBattlerDataComponent::IncrementActionCount()
{
	ActionCount++;
	for (UBattleState* State : AllStates)
	{
		State->OnActionCountIncremented(ActionCount);
	}
}

void UBattlerDataComponent::ResetActionCount()
{
	ActionCount = 0;
}

float UBattlerDataComponent::UpdateForceValue(float Value, float Percentage)
{
	return UpdateParameter(Force, Value, Percentage);
}

float UBattlerDataComponent::UpdateResistenceValue(float Value, float Percentage)
{
	return UpdateParameter(Resistence, Value, Percentage);
}

int32 UBattlerDataComponent::UpdateSpeedValue(float Value, float Percentage)
{
	float FloatingSpeed = Speed;
	Speed = UpdateParameter(FloatingSpeed, Value, Percentage);
	return Speed;
}

float UBattlerDataComponent::UpdateParameter(float& Attribute, float Value, float Percentage)
{
	if (Value == 0.f && Percentage == 0.f) return Attribute;
	float AdjustedPercentage = (Percentage / 100.f) * Attribute;
	Attribute += Value + AdjustedPercentage;
	Attribute = FMath::Max(Attribute, 0.f);
	return Attribute;
}

int32 UBattlerDataComponent::UpdateAdditionalActions(int32 Value)
{
	AdditionalActions += Value;
	AdditionalActions = FMath::Max(AdditionalActions, 0);
	return AdditionalActions;
}

int32 UBattlerDataComponent::UpdateAdditionalLives(int32 Value)
{
	AdditionalLives += Value;
	AdditionalLives = FMath::Max(AdditionalLives, 0);
	return AdditionalLives;
}

float UBattlerDataComponent::UpdateForceMultiplier(float Value)
{
	return UpdateMultiplier(ForceMultiplier, Value);
}

float UBattlerDataComponent::UpdateResistenceMultiplier(float Value)
{
	return UpdateMultiplier(ResistenceMultiplier, Value);
}

float UBattlerDataComponent::UpdateSpeedMultiplier(float Value)
{
	return UpdateMultiplier(SpeedMultiplier, Value);
}

float UBattlerDataComponent::UpdateDamageDealtMultiplier(float Value)
{
	return UpdateMultiplier(DamageDealtMultiplier, Value);
}

float UBattlerDataComponent::UpdateDamageReceivedMultiplier(float Value)
{
	return UpdateMultiplier(DamageReceivedMultiplier, Value);
}

float UBattlerDataComponent::UpdateHealMultiplier(float Value)
{
	return UpdateMultiplier(HealMultiplier, Value);
}

float UBattlerDataComponent::UpdateManaRestoreMultiplier(float Value)
{
	return UpdateMultiplier(ManaRestoreMultiplier, Value);
}

float UBattlerDataComponent::UpdateMultiplier(float& Multiplier, float Value)
{
	Multiplier += Value;
	Multiplier = FMath::Max(Multiplier, 0.f);
	return Multiplier;
}


