// Fill out your copyright notice in the Description page of Project Settings.


#include "Turnbased/TurnBasedBattle.h"
#include "Turnbased/BattleInputAction.h"
#include "Algo/Count.h"
#include "Turnbased/TurnBasedBattleWidget.h"
#include "Kismet/GameplayStatics.h"

ATurnBasedBattle::ATurnBasedBattle()
{
	AllyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Allies"));
	AllyRoot->SetupAttachment(BattleRoot);

	EnemyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Enemies"));
	EnemyRoot->SetupAttachment(BattleRoot);
}

void ATurnBasedBattle::BeginPlay()
{
	Super::BeginPlay();
	if (IsValid(BattleWidget))
	{
		TurnBattleWidget = Cast<UTurnBasedBattleWidget>(BattleWidget);
	}
}

void ATurnBasedBattle::StartBattle(float BlendTime)
{
	if (bIsBattleActive) return;
	Super::StartBattle(BlendTime);
	CurrentTurn = -1;
	CurrentAction = 0;
	Actions.Empty();
	GetWorldTimerManager().SetTimer(BattleTimer, this, &ATurnBasedBattle::StartTurn, FirstTurnDelay + BlendTime, false);
	if (IsValid(TurnBattleWidget))
	{
		TurnBattleWidget->SetBattleManager(this);
		TurnBattleWidget->OnBattleStarted();
	}
	ChangeBattleSpeed(BattleSpeed);
}

void ATurnBasedBattle::EvaluateTransforms()
{
	AllyTransforms.Empty();
	TArray<USceneComponent*> AllyComponents;
	AllyRoot->GetChildrenComponents(false, AllyComponents);
	if (AllyComponents.Num() > 0)
	{
		FVector AllyRootLocation = AllyRoot->GetComponentLocation();
		AllyComponents.Sort([AllyRootLocation](const USceneComponent& A, const USceneComponent& B)
			{
				return FVector::DistSquared(A.GetComponentLocation(), AllyRootLocation) < FVector::DistSquared(B.GetComponentLocation(), AllyRootLocation);
			});
		for (USceneComponent* Component : AllyComponents)
		{
			FTransform LocalTransform = Component->GetComponentTransform();
			AllyTransforms.Add(LocalTransform);
		}
	}
	EnemyTransforms.Empty();
	TArray<USceneComponent*> EnemyComponents;
	EnemyRoot->GetChildrenComponents(false, EnemyComponents);
	if (EnemyComponents.Num() > 0)
	{
		FVector EnemyRootLocation = EnemyRoot->GetComponentLocation();
		EnemyComponents.Sort([EnemyRootLocation](const USceneComponent& A, const USceneComponent& B)
			{
				return FVector::DistSquared(A.GetComponentLocation(), EnemyRootLocation) < FVector::DistSquared(B.GetComponentLocation(), EnemyRootLocation);
			});
		for (USceneComponent* Component : EnemyComponents)
		{
			FTransform LocalTransform = Component->GetComponentTransform();
			EnemyTransforms.Add(LocalTransform);
		}
	}
	TArray<USceneComponent*> AllComponents;
	AllComponents.Append(AllyComponents);
	AllComponents.Append(EnemyComponents);
	if (AllComponents.Num() > 0)
	{
		for (USceneComponent* Component : AllComponents)
		{
			Component->SetHiddenInGame(true, false);
		}
	}
}

void ATurnBasedBattle::LoadBattlers_Implementation()
{
	LoadSpawnBattlers(EnemyClasses, EBattlerType::Enemy, EnemyRoot, EnemyTransforms);
	LoadSpawnBattlers(AllyClasses, EBattlerType::Ally, AllyRoot, AllyTransforms);
	if (bHandleDuplicateAllyNames) HandleDuplicateNames(Allies);
	if (bHandleDuplicateEnemyNames) HandleDuplicateNames(Enemies);
	Super::LoadBattlers_Implementation();
}

bool ATurnBasedBattle::EvaluateEndBattle_Implementation()
{
	if (MaxTurns > 0 && CurrentTurn >= MaxTurns)
	{
		bIsVictorious = bVictoryWhenMaxTurns;
		EndBattle();
		return true;
	}
	return Super::EvaluateEndBattle_Implementation();
}

void ATurnBasedBattle::LoadSpawnBattlers(TArray<TSoftClassPtr<AActor>> Battlers, EBattlerType BattlerType, USceneComponent* BattlerRoot, TArray<FTransform> TransformGroup)
{
	if (!GetWorld() || !BattlerRoot) return;
	int32 Length = FMath::Min(Battlers.Num(), TransformGroup.Num());
	if (Length <= 0) return;
	for (int32 Index = 0; Index < Length; Index++)
	{
		TSubclassOf<AActor> LoadedBattler = Battlers[Index].LoadSynchronous();
		if (!LoadedBattler) continue;
		AActor* SpawnedBattler = SpawnBattler(LoadedBattler, TransformGroup[Index], BattlerType, BattlerRoot, true);
		if (IsValid(SpawnedBattler))
		{
			AllBattlers.Add(SpawnedBattler);
		}
	}
}

void ATurnBasedBattle::HandleDuplicateNames(const TArray<AActor*>& Battlers)
{
	if (Battlers.Num() == 0) return;
	TMap<FString, TArray<UBattlerDataComponent*>> NameToBattlers;
	for (AActor* Battler : Battlers)
	{
		if (IsValid(Battler))
		{
			UBattlerDataComponent* Data = UBattlerDataComponent::GetBattlerDataComponent(Battler);
			if (IsValid(Data))
			{
				FString NameKey = Data->GetCharacterName().ToString(); 
				NameToBattlers.FindOrAdd(NameKey).Add(Data);
			}
		}
	}
	for (auto& Pair : NameToBattlers)
	{
		const FString& BaseName = Pair.Key;
		TArray<UBattlerDataComponent*>& BattlersWithName = Pair.Value;
		if (BattlersWithName.Num() > 1)
		{
			for (int32 i = 0; i < BattlersWithName.Num(); ++i)
			{
				FString Suffix = bUseAlphabeticalSuffix
					? FString::Printf(TEXT("%c"), 'A' + i)
					: FString::FromInt(i + 1);
				FText UniqueName = FText::FromString(BaseName + " " + Suffix);
				BattlersWithName[i]->SetCharacterName(UniqueName);
			}
		}
	}
}

void ATurnBasedBattle::GetAlliesFromArray(TArray<TSoftClassPtr<AActor>> NewAllies)
{
	if (NewAllies.Num() <= 0) return;
	AllyClasses.Empty();
	AllyClasses = NewAllies;
}

void ATurnBasedBattle::GetEnemiesFromArray(TArray<TSoftClassPtr<AActor>> NewEnemies)
{
	if (NewEnemies.Num() <= 0) return;
	EnemyClasses.Empty();
	EnemyClasses = NewEnemies;
}

TArray<TSoftClassPtr<AActor>> ATurnBasedBattle::LoadEnemiesGroup(int32 Index)
{
	if (EnemyGroups.IsValidIndex(Index))
	{
		FEnemyGroups Group = EnemyGroups[Index];
		if (Group.EnemyGroup.Num() > 0)
		{
			GetEnemiesFromArray(Group.EnemyGroup);
			CurrentEnemyGroup = Index;
			return Group.EnemyGroup;
		}
	}
	return TArray<TSoftClassPtr<AActor>>();
}

TArray<TSoftClassPtr<AActor>> ATurnBasedBattle::LoadRandomEnemyGroup()
{
	if (EnemyGroups.Num() == 0) return TArray<TSoftClassPtr<AActor>>();
	int32 RandomIndex = FMath::RandRange(0, EnemyGroups.Num() - 1);
	return LoadEnemiesGroup(RandomIndex);
}

FTransform ATurnBasedBattle::GetFreeAllyTransform() const
{
	return GetFreeTransform(EBattlerType::Ally);
}

FTransform ATurnBasedBattle::GetFreeEnemyTransform() const
{
	return GetFreeTransform(EBattlerType::Enemy);
}

FTransform ATurnBasedBattle::GetFreeTransform(EBattlerType BattlerType) const
{
	TArray<AActor*> BattlerGroup;
	TArray<FTransform> TransformGroup;
	switch (BattlerType)
	{
	case EBattlerType::None:
		return FTransform();
	case EBattlerType::Ally:
		BattlerGroup = Allies;
		TransformGroup = AllyTransforms;
		break;
	case EBattlerType::Enemy:
		BattlerGroup = Enemies;
		TransformGroup = EnemyTransforms;
		break;
	default:
		return FTransform();
	}
	if (BattlerGroup.Num() == 0 || TransformGroup.Num() == 0)
	{
		return FTransform();
	}
	for (const FTransform& Transform : TransformGroup)
	{
		bool bIsOccupied = false;
		FVector TransformLocation = Transform.GetLocation();
		for (AActor* Battler : BattlerGroup)
		{
			if (IsValid(Battler))
			{
				if (bIgnoreZForTransform)
				{
					bIsOccupied = FVector::DistXY(Battler->GetActorLocation(), TransformLocation) < TransformDistanceTolerance;
				}
				else
				{
					bIsOccupied = FVector::Dist(Battler->GetActorLocation(), TransformLocation) < TransformDistanceTolerance;
				}
				if (bIsOccupied) break;
			}
		}
		if (!bIsOccupied)
		{
			return Transform;
		}
	}
	return FTransform();
}

void ATurnBasedBattle::ChangeBattleSpeed(float NewSpeed)
{
	if (NewSpeed <= 0.f) return;
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), NewSpeed);
}

AActor* ATurnBasedBattle::SpawnEnemy(TSubclassOf<AActor> EnemyClass, bool bSpawnOnlyIfEmpty)
{
	FTransform EmptyTransform = GetFreeEnemyTransform();
	if (bSpawnOnlyIfEmpty && EmptyTransform.Equals(FTransform())) return nullptr;
	AActor* Spawned = SpawnBattler(EnemyClass, EmptyTransform, EBattlerType::Enemy, EnemyRoot);
	if (IsValid(Spawned))
	{
		EnemyDamage.Add(0.f);
	}
	return Spawned;
}

AActor* ATurnBasedBattle::SpawnAlly(TSubclassOf<AActor> AllyClass, bool bSpawnOnlyIfEmpty)
{
	FTransform EmptyTransform = GetFreeAllyTransform();
	if (bSpawnOnlyIfEmpty && EmptyTransform.Equals(FTransform())) return nullptr;
	AActor* Spawned = SpawnBattler(AllyClass, EmptyTransform, EBattlerType::Ally, AllyRoot);
	if (IsValid(Spawned))
	{
		AllyDamage.Add(0.f);
	}
	return Spawned;
}

UTurnBasedBattleWidget* ATurnBasedBattle::GetTurnBattleWidget() const
{
	return TurnBattleWidget;
}

ATurnBasedBattle* ATurnBasedBattle::FindActiveTurnBattle(const UObject* WorldContext, int32& BattleId)
{
	return Cast<ATurnBasedBattle>(ASweetDreamsBattleManager::FindActiveBattle(WorldContext, BattleId));
}

ATurnBasedBattle* ATurnBasedBattle::FindTurnBattleByIndex(const UObject* WorldContext, int32 Index)
{
	return Cast<ATurnBasedBattle>(ASweetDreamsBattleManager::FindBattleByIndex(WorldContext, Index));
}

FString ATurnBasedBattle::GetBattlerBaseName(const FString& Name)
{
	if (Name.Len() > 2 && FChar::IsAlpha(Name.Right(1)[0]) && Name.Right(2).Left(1) == " ")
	{
		return Name.LeftChop(2);
	}
	return Name;
}

void ATurnBasedBattle::StartTurn()
{
	if (bBattlePaused || EvaluateEndBattle()) return;
	CurrentTurn++;
	for (AActor* Battler : AllBattlers)
	{
		if (IsValid(Battler))
		{
			UBattlerDataComponent* Data;
			Data = Battler->FindComponentByClass<UBattlerDataComponent>();
			if (IsValid(Data))
			{
				Data->UpdateTurnActionsCooldown();
				Data->ResetActionCount();
				TArray<UBattleElement*> AllElements = Data->GetAllElements();
				for (UBattleElement* Element : AllElements)
				{
					Element->OnTurnStart(CurrentTurn);
				}
				TArray<UBattleState*> States = Data->GetAllStates();
				if (States.Num() > 0)
				{
					for (UBattleState* State : States)
					{
						State->ConsumeLifetime(EStateLifetime::Turn);
					}
				}
			}
		}
	}
	for (UBattleAction* Action : Actions)
	{
		if (IsValid(Action))
		{
			Action->ResetSkipAction();
		}
	}
	CurrentAction = 0;
	Actions.Empty();
	if (bAutoLoadAllyActions) LoadTurnActions(Allies, true);
	if (bAutoLoadEnemyActions) LoadTurnActions(Enemies, false);
	StartTurnAction();
	OnTurnStarted(CurrentTurn);
}

void ATurnBasedBattle::LoadTurnActions(TArray<AActor*> Battlers, bool bIsAlly)
{
	for (AActor* Battler : Battlers)
	{
		if (IsValid(Battler))
		{
			UBattlerDataComponent* Data;
			Data = Battler->FindComponentByClass<UBattlerDataComponent>();
			if (IsValid(Data) && Data->GetIsAbleToAct() && !Data->IsDead())
			{
				int32 ActionsToAdd = Data->GetActionsPerTurn();
				for (int32 Index = 0; Index < ActionsToAdd; Index++)
				{
					if (bIsAlly)
					{
						UBattleInputAction* InputAction = NewObject<UBattleInputAction>(Battler, InputActionClass);
						InputAction->SetOwner(Battler);
						AddTurnAction(InputAction, false);
					}
					else
					{
						UBattleAction* EnemyAction = Data->GetRandomAction();
						if (IsValid(EnemyAction))
						{
							EnemyAction->SetCurrentBattle(this);
							EnemyAction->LoadRandomTurnTargets();
							AddTurnAction(EnemyAction);
							EnemyAction->EvaluateCooldown();
						}
					}
				}
			}
		}
	}
}

void ATurnBasedBattle::AddTurnAction(UBattleAction* Action, bool bIgnoreSpeed, int32 IndexToAdd)
{
	if (!IsValid(Action) || !IsValid(Action->GetOwner())) return;
	Action->SetCurrentBattle(this);
	if (Actions.Num() == 0)
	{
		Actions.Add(Action);
		OnActionAdded(Action);
		return;
	}
	if (!bIgnoreSpeed)
	{
		int32 InsertIndex = CurrentAction;
		for (int32 i = CurrentAction; i < Actions.Num(); ++i)
		{
			if (Action->GetActionSpeed() > Actions[i]->GetActionSpeed())
			{
				InsertIndex = i;
				break;
			}
		}
		if (InsertIndex == CurrentAction && Action->GetActionSpeed() <= Actions.Last()->GetActionSpeed())
		{
			InsertIndex = Actions.Num();
		}
		Actions.Insert(Action, InsertIndex);
	}
	else
	{
		if (IndexToAdd != -1 && Actions.IsValidIndex(IndexToAdd))
		{
			Actions.Insert(Action, IndexToAdd);
		}
		Actions.Add(Action);
	}
	OnActionAdded(Action);
}

bool ATurnBasedBattle::RemoveTurnAction(UBattleAction* Action, int32& ActionCount)
{
	ActionCount = 0;
	if (!IsValid(Action)) return false;
	if (Actions.Find(Action) != INDEX_NONE)
	{
		OnActionRemoved(Action);
		ActionCount = Algo::Count(Actions, Action);
		return true;
	}
	return false;
}

void ATurnBasedBattle::StartTurnAction()
{
	if (Actions.Num() <= 0 || bBattlePaused || EvaluateEndBattle()) return;
	while (CurrentAction < Actions.Num())
	{
		UBattleAction* CurrentActionRef = Actions[CurrentAction++];
		CurrentActionBattler = CurrentActionRef->GetOwner();
		if (IsValid(CurrentActionBattler))
		{
			UBattlerDataComponent* BattlerData = CurrentActionBattler->FindComponentByClass<UBattlerDataComponent>();
			if (IsValid(BattlerData) && !BattlerData->IsDead())
			{
				if (bAutoMoveCamera) ChangeCameraFocus(CurrentActionBattler, BattlerBlendTime);
				FTimerHandle LocalHandle;
				FTimerDelegate TimerDel;
				TimerDel.BindUFunction(CurrentActionRef, FName("StartAction"), true);
				GetWorldTimerManager().SetTimer(LocalHandle, TimerDel, BattlerBlendTime + ActionDelay, false);
				return;
			}
		}
	}
	StartTurn();
}

bool ATurnBasedBattle::TurnContainsAction(UBattleAction* Action, int32& Amount) const
{
	Amount = Algo::Count(Actions, Action);
	return Actions.Contains(Action);
}

bool ATurnBasedBattle::TurnContainsActionOfClass(TSubclassOf<UBattleAction> Action, int32& Amount) const
{
	Amount = Algo::CountIf(Actions, [Action](const UBattleAction* Act)
	{
		return Act && Act->IsA(Action);
	});
	return Amount > 0;
}

void ATurnBasedBattle::EndBattle(float BlendTime)
{
	if (TurnBattleWidget) TurnBattleWidget->OnBattleEnded(bIsVictorious);
	for (AActor* Battler : AllBattlers)
	{
		UBattlerDataComponent* Data;
		Data = Battler->FindComponentByClass<UBattlerDataComponent>();
		if (IsValid(Data))
		{
			Data->ResetActions();
		}
	}
	ChangeBattleSpeed(1.f);
	Super::EndBattle(BlendTime);
}

void ATurnBasedBattle::SetBattlePaused(bool bPaused)
{
	if (bBattlePaused == bPaused) return;
	bPaused == true ? PauseTurnBattle() : UnpauseTurnBattle();
}

void ATurnBasedBattle::PauseTurnBattle()
{
	bBattlePaused = true;
}

void ATurnBasedBattle::UnpauseTurnBattle()
{
	bBattlePaused = false;
	CurrentAction < Actions.Num() == true ? StartTurnAction() : StartTurn();
}


