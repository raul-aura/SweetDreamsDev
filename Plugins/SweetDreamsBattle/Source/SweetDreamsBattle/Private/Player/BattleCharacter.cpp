// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/BattleCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Core/SweetDreamsBPLibrary.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/BattleNumberWidget.h"
#include "Player/BattlerParameterWidget.h"
#include "Battle/SweetDreamsBattleManager.h"

ABattleCharacter::ABattleCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	CameraBoom->bUsePawnControlRotation = false;
	CameraBoom->bInheritPitch = false;
	CameraBoom->bInheritYaw = false;
	CameraBoom->bInheritRoll = false;

	BattleWorldArea = CreateDefaultSubobject<UBoxComponent>("Battle World Area");
	BattleWorldArea->SetupAttachment(RootComponent);
	ParameterIndicator = CreateDefaultSubobject<UWidgetComponent>("Parameter Indicator");
	ParameterIndicator->SetupAttachment(RootComponent);
	ParameterIndicator->SetDrawSize(FVector2D(1000.f));
	ParameterIndicator->SetTwoSided(true);
	ParameterIndicator->SetPivot(FVector2D(0.5f));

	ParameterIndicatorClass = UBattlerParameterWidget::StaticClass();
	DamageIndicatorClass = UBattleNumberWidget::StaticClass();

	GetCharacterMovement()->MaxWalkSpeed = 900.f;
}

void ABattleCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ParameterIndicator->SetWidgetClass(ParameterIndicatorClass);
}

void ABattleCharacter::BeginPlay()
{
	Super::BeginPlay();
	UBattlerParameterWidget* ParameterWidget = Cast<UBattlerParameterWidget>(ParameterIndicator->GetWidget());
	if (IsValid(ParameterWidget))
	{
		ParameterWidget->DefineActor(this);
	}
	if (GetBattlerParameters())
	{
		IndicateParameter(GetBattlerParameters());
	}
}

UBattlerDataComponent* ABattleCharacter::GetBattlerParameters() const
{
	return FindComponentByClass<UBattlerDataComponent>();
}

void ABattleCharacter::OnAttack_Implementation()
{
	if (bAttackOnCooldown) return;
	if (bAttackStopsMovement) bCanMove = false;
	bIsAttacking = true;
	bAttackOnCooldown = true;
	FTimerHandle AttackCooldownTimer;
	FTimerHandle LocalTimer;
	GetWorldTimerManager().SetTimer(AttackCooldownTimer, [this] {
		bAttackOnCooldown = false;
		if (bAttackStopsMovement) bCanMove = true;
		}, AttackCooldown, false);
	GetWorldTimerManager().SetTimer(LocalTimer, [this] {
		bIsAttacking = false;
		}, GetWorld()->GetDeltaSeconds(), false);
}

void ABattleCharacter::OnDamageReceived_Implementation(float Damage)
{
	FTimerHandle AttackTimer;
	bIsAttacked = true;
	GetWorldTimerManager().SetTimer(AttackTimer, [this]() {
		bIsAttacked = false;
		}, GetWorld()->GetDeltaSeconds(), false);
}

float ABattleCharacter::OnCharacterMitigateDamage_Implementation(float Damage)
{
	return Damage;
}

void ABattleCharacter::IndicateDamage_Implementation(float Value, bool bIsHealInstead)
{
	TSubclassOf<UBattleNumberWidget> IndicatorClass = nullptr;
	if (bOverrideBattleIndicator && IsValid(DamageIndicatorClass))
	{
		IndicatorClass = DamageIndicatorClass;
	}
	else
	{
		int32 Temp = 0;
		ASweetDreamsBattleManager* Battle = ASweetDreamsBattleManager::FindActiveBattle(this, Temp);
		if (IsValid(Battle))
		{
			IndicatorClass = Battle->GetDamageIndicatorClass();
		}
	}
	if (!IsValid(IndicatorClass)) return;
	FVector Origin = BattleWorldArea->GetComponentLocation();
	FVector Extent = BattleWorldArea->GetUnscaledBoxExtent() - IndicatorPadding;
	FVector DamageLocation = UKismetMathLibrary::RandomPointInBoundingBox(Origin, Extent);
	UWidgetComponent* DamageIndicator = NewObject<UWidgetComponent>(BattleWorldArea);
	if (!DamageIndicator) return;
	DamageIndicator->SetWidgetClass(IndicatorClass);
	DamageIndicator->SetWidgetSpace(EWidgetSpace::Screen);
	DamageIndicator->SetDrawSize(FVector2D(1000.f));
	DamageIndicator->SetTwoSided(true);
	DamageIndicator->SetDrawAtDesiredSize(true);
	DamageIndicator->AttachToComponent(BattleWorldArea, FAttachmentTransformRules::SnapToTargetIncludingScale);
	DamageIndicator->RegisterComponent();
	DamageIndicator->SetWorldLocation(DamageLocation);
	UBattleNumberWidget* DamageWidget = Cast<UBattleNumberWidget>(DamageIndicator->GetWidget());
	if (IsValid(DamageWidget))
	{
		DamageWidget->OwningWidgetComponent = DamageIndicator;
		DamageWidget->OwningBattler = this;
		bIsHealInstead ? DamageWidget->IndicateHeal(Value) : DamageWidget->IndicateDamage(Value);
	}
}

void ABattleCharacter::RemoveDamageIndicator(UWidgetComponent* Component)
{
	Component->DestroyComponent();
}

void ABattleCharacter::IndicateParameter_Implementation(UBattlerDataComponent* BattlerParameters)
{
	UBattlerParameterWidget* ParameterWidget = Cast<UBattlerParameterWidget>(ParameterIndicator->GetWidget());
	if (IsValid(ParameterWidget)) ParameterWidget->UpdateParameters(BattlerParameters);
}





