// Fill out your copyright notice in the Description page of Project Settings.

#include "SweetDreamsBattleManager.h"
#include "Kismet/GameplayStatics.h"
#include "SweetDreamsWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SweetDreamsGameMode.h"

ASweetDreamsBattleManager::ASweetDreamsBattleManager()
{
	PrimaryActorTick.bCanEverTick = true;
	BattleRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Battle Manager"));
	BattleRoot->SetupAttachment(RootComponent);

	BattleCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Battle Camera"));
	BattleCamera->SetupAttachment(BattleRoot);

	MulticameraComponent = CreateDefaultSubobject<UMulticameraComponent>(TEXT("Multicamera Component"));
	AddOwnedComponent(MulticameraComponent);

	DamageIndicatorClass = UBattleNumberWidget::StaticClass();
}

void ASweetDreamsBattleManager::BeginPlay()
{
	Super::BeginPlay();
	Player = UGameplayStatics::GetPlayerController(this, 0);
	BattleWidget = ASweetDreamsHUD::FindWidgetByName(BattleWidgetName);
}

void ASweetDreamsBattleManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASweetDreamsBattleManager::StartBattleByIndex(const UObject* WorldContext, int32 Index)
{
	ASweetDreamsBattleManager* Battle = FindBattleByIndex(WorldContext, Index);
	if (Battle)
	{
		Battle->StartBattle();
	}
}

ASweetDreamsBattleManager* ASweetDreamsBattleManager::FindBattleByIndex(const UObject* WorldContext, int32 Index)
{
	if (!ensureAlwaysMsgf(IsValid(WorldContext), TEXT("World Context was not valid.")) || Index < 0)
	{
		return nullptr;
	}
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(WorldContext, ASweetDreamsBattleManager::StaticClass(), FoundActors);
	for (AActor* Actor : FoundActors)
	{
		ASweetDreamsBattleManager* BattleManager = Cast<ASweetDreamsBattleManager>(Actor);
		if (BattleManager && BattleManager->BattleIndex == Index)
		{
			return BattleManager;
		}
	}
	return nullptr;
}

ASweetDreamsBattleManager* ASweetDreamsBattleManager::FindActiveBattle(const UObject* WorldContext, int32& BattleId)
{
	BattleId = -1;
	if (!ensureAlwaysMsgf(IsValid(WorldContext), TEXT("World Context was not valid.")))
	{
		return nullptr;
	}
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(WorldContext, ASweetDreamsBattleManager::StaticClass(), FoundActors);
	for (AActor* Actor : FoundActors)
	{
		ASweetDreamsBattleManager* BattleManager = Cast<ASweetDreamsBattleManager>(Actor);
		if (BattleManager && BattleManager->bIsBattleActive)
		{
			BattleId = BattleManager->BattleIndex;
			return BattleManager;
		}
	}
	return nullptr;
}

void ASweetDreamsBattleManager::SetBattlePaused(bool bPaused)
{
	bBattlePaused = bPaused;
}

void ASweetDreamsBattleManager::StartBattle(float BlendTime)
{
	if (bIsBattleActive) return;
	AllyDamage.Empty();
	EnemyDamage.Empty();
	LoadBattlers();
	bIsBattleActive = true;
	if (IsValid(Player))
	{
		Player->StopMovement();
		ChangeCameraFocus(this, BlendTime);
	}
	if (IsValid(BattleWidget))
	{
		ASweetDreamsHUD::ShowWidget(BattleWidget);
	}
	OnBattleStart();
}

AActor* ASweetDreamsBattleManager::SpawnBattler(TSubclassOf<AActor> Battler, FTransform Transform, EBattlerType BattlerType, USceneComponent* Root, bool bAddToReferences)
{
	if (IsValid(Battler))
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AActor* SpawnedBattler = GetWorld()->SpawnActor<AActor>(Battler, SpawnParams);
		if (IsValid(SpawnedBattler))
		{
			SpawnedBattler->SetActorTransform(Transform, false, nullptr, ETeleportType::TeleportPhysics);
			if (IsValid(Root)) SpawnedBattler->AttachToComponent(Root, FAttachmentTransformRules::KeepWorldTransform);
			if (bAddToReferences)
			{
				TArray<AActor*>& TargetGroup = GetBattlerGroup(BattlerType);
				TargetGroup.Add(SpawnedBattler);
			}
			UBattlerDataComponent* Data = UBattlerDataComponent::GetBattlerDataComponent(SpawnedBattler);
			if (IsValid(Data))
			{
				Data->SetInBattle(this, true);
			}
		}
		return SpawnedBattler;
	}
	return nullptr;
}

void ASweetDreamsBattleManager::LoadBattlers_Implementation()
{
	EnemyDamage.Init(0.0f, Enemies.Num());
	AllyDamage.Init(0.0f, Allies.Num());
	TArray<AActor*> AllBattlers = Allies;
	AllBattlers.Append(Enemies);
	for (AActor* Battler : AllBattlers)
	{
		UBattlerDataComponent* Data = UBattlerDataComponent::GetBattlerDataComponent(Battler);
		if (IsValid(Data))
		{
			Data->SetInBattle(this, true);
			TArray<UBattleElement*> AllElements = Data->GetAllElements();
			for (UBattleElement* Element : AllElements)
			{
				if (IsValid(Element)) Element->OnBattleStart(this);
			}
		}
	}
}

void ASweetDreamsBattleManager::EndBattle(float BlendTime)
{
	bIsBattleActive = false;
	if (APawn* PlayerPawn = Player->GetPawn())
	{
		ChangeCameraFocus(PlayerPawn, BlendTime);
	}
	if (IsValid(BattleWidget))
	{
		ASweetDreamsHUD::HideWidget(BattleWidget);
	}
	TArray<AActor*> AllBattlers = Allies;
	AllBattlers.Append(Enemies);
	for (AActor* Battler : AllBattlers)
	{
		UBattlerDataComponent* Data = UBattlerDataComponent::GetBattlerDataComponent(Battler);
		if (IsValid(Data))
		{
			Data->SetInBattle(this, false);
			TArray<UBattleElement*> AllElements = Data->GetAllElements();
			for (UBattleElement* Element : AllElements)
			{
				if (IsValid(Element)) Element->OnBattleEnd(this);
			}
		}
	}
	OnBattleEnd(bIsVictorious);
}

bool ASweetDreamsBattleManager::EvaluateEndBattle_Implementation()
{ 
	if (!bIsBattleActive) return false;
	bool bAllEnemiesDead = true;
	bool bAllAlliesDead = true;
	if (Enemies.Num() > 0)
	{
		for (AActor* Enemy : Enemies)
		{
			UBattlerDataComponent* Data;
			if (Cast<ABattleCharacter>(Enemy))
			{
				Data = Cast<ABattleCharacter>(Enemy)->GetBattlerParameters();
			}
			else
			{
				Data = Enemy->FindComponentByClass<UBattlerDataComponent>();
			}
			if (Data && !Data->IsDead())
			{
				bAllEnemiesDead = false;
				break;
			}
		}
	}
	if (Allies.Num() > 0)
	{
		for (AActor* Ally : Allies)
		{
			UBattlerDataComponent* Data;
			if (Cast<ABattleCharacter>(Ally))
			{
				Data = Cast<ABattleCharacter>(Ally)->GetBattlerParameters();
			}
			else
			{
				Data = Ally->FindComponentByClass<UBattlerDataComponent>();
			}
			if (Data && !Data->IsDead())
			{
				bAllAlliesDead = false;
				break;
			}
		}
	}
	if (bAllEnemiesDead || bAllAlliesDead)
	{
		if (bAllEnemiesDead)
		{
			bIsVictorious = true;
		}
		else
		{
			bIsVictorious = false;
		}
		EndBattle();
		return true;
	}
	return false;
}

bool ASweetDreamsBattleManager::IsBattleOngoing() const
{
	return bIsBattleActive;
}

bool ASweetDreamsBattleManager::IsBattleVictorious() const
{
	return bIsVictorious;
}

void ASweetDreamsBattleManager::ChangeCameraFocus(AActor* NewFocus, float BlendTime)
{
	if (!Player || !NewFocus) return;
	if (BlendTime < 0.0f)
	{
		BlendTime = BattlerBlendTime;
	}
	Player->SetViewTargetWithBlend(NewFocus, BlendTime);
}

void ASweetDreamsBattleManager::ChangeCameraView(ECameraView NewView, AActor* SelfFocus, float BlendTime)
{
	int32 Index = static_cast<int32>(NewView);
	if (NewView == ECameraView::Self)
	{
		Index = 1;
	}
	else if ((NewView != ECameraView::Self) || !IsValid(SelfFocus))
	{
		SelfFocus = this;
	}
	ChangeCameraFocus(SelfFocus, BlendTime);
	UMulticameraComponent* Multicamera = SelfFocus->FindComponentByClass<UMulticameraComponent>();
	if (IsValid(Multicamera) && Multicamera->GetAllPossibleViews().IsValidIndex(Index))
	{
		Multicamera->SetNewCameraView(Index, BlendTime);
	}
}

void ASweetDreamsBattleManager::AddDamageToBattle(AActor* DamageOwner, float Damage, bool bApplyCalculations)
{
	if (!DamageOwner || Damage <= 0.0f) return;
	int32 Index = Enemies.Find(DamageOwner);
	if (EnemyDamage.IsValidIndex(Index))
	{
		EnemyDamage[Index] += Damage;
		OnDamageApplied(DamageOwner, Damage, false, bApplyCalculations);
		return;
	}
	Index = Allies.Find(DamageOwner);
	if (AllyDamage.IsValidIndex(Index))
	{
		AllyDamage[Index] += Damage;
	}
	OnDamageApplied(DamageOwner, Damage, true, bApplyCalculations);
}

float ASweetDreamsBattleManager::GetAllAlliedDamage() const
{
	float Damage = 0.f;
	if (AllyDamage.Num() > 0)
	{
		for (float Instance : AllyDamage)
		{
			Damage += Instance;
		}
	}
	return Damage;
}

float ASweetDreamsBattleManager::GetAllEnemyDamage() const
{
	float Damage = 0.f;
	if (EnemyDamage.Num() > 0)
	{
		for (float Instance : EnemyDamage)
		{
			Damage += Instance;
		}
	}
	return Damage;
}

TArray<AActor*>& ASweetDreamsBattleManager::GetBattlerGroup(EBattlerType BattlerType)
{
	switch (BattlerType)
	{
	case EBattlerType::Ally:
		return Allies;
	case EBattlerType::Enemy:
		return Enemies;
	default:
		checkNoEntry();
		return Allies;
	}
}

TArray<AActor*> ASweetDreamsBattleManager::GetAllPossibleTargets(UBattleAction* Action, bool bUpdateCameraView)
{
	auto FilterTargets = [](TArray<AActor*>& Actors, bool (*FilterFn)(AActor*)) {
		TArray<AActor*> FilteredTargets;
		for (AActor* Actor : Actors)
		{
			if (FilterFn(Actor))
			{
				FilteredTargets.Add(Actor);
			}
		}
		Actors = MoveTemp(FilteredTargets);
	};
	TArray<AActor*> Targets;
	ECameraView NewView = ECameraView::AllBattlers;
	const bool bIsEnemy = IsActorEnemy(Action->GetOwner());
	switch (Action->GetTargetType())
	{
	case ETargetType::Ally:
	case ETargetType::AllAlly:
		Targets = bIsEnemy ? Enemies : Allies;
		NewView = bIsEnemy ? ECameraView::AllBattlers : ECameraView::Allies;
		break;
	case ETargetType::DeadAlly:
		Targets = bIsEnemy ? Enemies : Allies;
		NewView = ECameraView::Allies;
		FilterTargets(Targets, [](AActor* Actor) {
			if (UBattlerDataComponent* Data = Actor->FindComponentByClass<UBattlerDataComponent>())
			{
				return Data->IsDead();
			}
			return false;
			});
		break;
	case ETargetType::Enemy:
	case ETargetType::AllEnemy:
		Targets = bIsEnemy ? Allies : Enemies;
		NewView = ECameraView::Enemies;
		break;
	case ETargetType::Self:
		Targets = Action->GetOwnerAsArray();
		NewView = ECameraView::Self;
		break;
	default:
		return {}; 
	}
	if (!Action->GetIfIncludeSelf())
	{
		Targets.Remove(Action->GetOwner());
	}
	if (bUpdateCameraView && bAutoMoveCamera)
	{
		ChangeCameraView(NewView, Action->GetOwner(), BattlerBlendTime);
	}
	return Targets;
}

bool ASweetDreamsBattleManager::IsActorAlly(const AActor* Actor) const
{
	return IsValid(Actor) && Allies.Contains(Actor);
}

bool ASweetDreamsBattleManager::IsActorEnemy(const AActor* Actor) const
{
	return IsValid(Actor) && Enemies.Contains(Actor);
}

TSubclassOf<UBattleNumberWidget> ASweetDreamsBattleManager::GetDamageIndicatorClass() const
{
	return DamageIndicatorClass;
}

