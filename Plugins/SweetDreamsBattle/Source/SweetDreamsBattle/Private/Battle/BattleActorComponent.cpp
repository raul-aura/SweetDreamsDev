
#include "Battle/BattleActorComponent.h"

UBattleActorComponent::UBattleActorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

UBattleActorComponent* UBattleActorComponent::GetBattleActorComponent(const AActor* Actor)
{
	UBattleActorComponent* Component = Actor->FindComponentByClass<UBattleActorComponent>();
	return Component;
}

void UBattleActorComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UBattleActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UBattleActorComponent::Damage(AActor* Target, float Amount)
{
	if (UBattleActorComponent* Component = GetBattleActorComponent(Target))
	{
		Component->ReceiveDamage(GetOwner(), Amount);
	}
}

void UBattleActorComponent::ReceiveDamage(AActor* Instigator, float Amount)
{
	// TODO: modify parameter, in this case, the Health parameter
}

void UBattleActorComponent::Heal(AActor* Target, float Amount)
{
	if (UBattleActorComponent* Component = GetBattleActorComponent(Target))
	{
		Component->ReceiveHeal(GetOwner(), Amount);
	}
}

void UBattleActorComponent::ReceiveHeal(AActor* Instigator, float Amount)
{
	// TODO: modify parameter, in this case, the Health parameter
}

void UBattleActorComponent::Kill()
{
	if (IsAlive())
	{
		bIsAlive = false;
	}
}

void UBattleActorComponent::Kill(AActor* Target)
{
	if (UBattleActorComponent* Component = GetBattleActorComponent(Target))
	{
		Component->Kill();
	}
}

void UBattleActorComponent::Ressurect()
{
	if (!IsAlive())
	{
		bIsAlive = true;
	}
}

void UBattleActorComponent::Ressurect(AActor* Target)
{
	if (UBattleActorComponent* Component = GetBattleActorComponent(Target))
	{
		Component->Ressurect();
	}
}

void UBattleActorComponent::SetInCombat(bool bInIsInCombat)
{
	bIsInCombat = bInIsInCombat;
}

bool UBattleActorComponent::IsAlive() const
{
	return bIsAlive;
}

bool UBattleActorComponent::IsInCombat() const
{
	return bIsInCombat;
}


