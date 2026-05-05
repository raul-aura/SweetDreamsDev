
#include "Battle/BattleActorComponent.h"
#include "Battle/SweetDreamsBattleBPLibrary.h"
#include "Battle/SweetDreamsBattleInterface.h"
#include "Battle/SweetDreamsBattleManager.h"
#include "Data/BattleElement.h"
#include "Data/BattleEvent.h"
#include "Data/BattleParameterEvent.h"

UBattleActorComponent::UBattleActorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

UBattleActorComponent* UBattleActorComponent::GetBattleActorComponent(const AActor* Actor)
{
	return Actor ? Actor->FindComponentByClass<UBattleActorComponent>() : nullptr;
}

void UBattleActorComponent::BeginPlay()
{
	Super::BeginPlay();

	for (TPair<FGameplayTag, FBattleParameter>& Parameter : Parameters)
	{
		InitializeParameter(Parameter.Value);
	}

	for (TObjectPtr<UBattleElementData> Data : InitialBattleElements)
	{
		TArray<UBattleActorComponent*> OutTargets;
		UBattleElement* Element = UBattleElement::CreateBattleElement(this, Data, OutTargets, nullptr, nullptr);
		RegisterBattleElement(Element);
	}
}

void UBattleActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	for (int32 i = BattleElements.Num() - 1; i >= 0; --i)
	{
		if (UBattleElement* Element = BattleElements[i])
		{
			Element->Tick(DeltaTime);
		}
	}
}

bool UBattleActorComponent::ExecuteBattleElement(const UBattleElementData* ElementData, UBattleElement*& OutElement)
{
	for (UBattleElement* ElementPtr : GetBattleElements())
	{
		if (ElementPtr->BattleElementData == ElementData)
		{
			if (CanExecuteBattleElement(ElementPtr))
			{
				ElementPtr->Execute();
				OutElement = ElementPtr;
				OnBattleElementExecuted_Event(ElementPtr);
				OnBattleElementExecuted.Broadcast(this, ElementPtr);

				return true;
			}
			else
			{
				break;
			}
		}
	}

	return false;
}

bool UBattleActorComponent::RegisterBattleElement(UBattleElement* BattleElement)
{
	if (IsValid(BattleElement) && !BattleElements.Contains(BattleElement))
	{
		BattleElements.AddUnique(BattleElement);
		if (BattleElement->bUnregisterOnEnd)
		{
			BattleElement->OnBattleElementEnd.BindUObject(this, &UBattleActorComponent::UnregisterBattleElement);
		}

		return true;
	}

	return false;
}

void UBattleActorComponent::UnregisterBattleElement(UBattleElement* BattleElement)
{
	if (IsValid(BattleElement))
	{
		if (BattleElements.Contains(BattleElement))
		{
			// TO DO: make event remove itself when removed from battle actor component
			OnBattleElementExecuted.RemoveAll(BattleElement);

			BattleElements.Remove(BattleElement);
		}
	}
}

FDamageHealTargetResult UBattleActorComponent::ReceiveDamage(UBattleActorComponent* Instigator, FGameplayTagContainer DamageTags, float Value)
{
	FDamageHealTargetResult Result;
	Result.Target = this;

	if (!IsAlive())
	{
		return Result;
	}

	if (FBattleParameter* Health = Parameters.Find(HealthParameterTag))
	{
		const float Previous = Health->GetResourceValue();

		Value = ModifyDamageReceived(Value);
		Health->RemoveResourceValue(Value);

		const float NewValue = Health->GetResourceValue();
		const float AppliedDamage = Previous - Health->GetResourceValue();

		Result.AppliedValue = AppliedDamage;

		if (NewValue <= 0.f)
		{
			SetIsAlive(false);
			Result.bKilled = true;
		}

		OnDamageReceived.Broadcast(this, Instigator, DamageTags, AppliedDamage, Previous, NewValue);
	}

	return Result;
}

FDamageHealTargetResult UBattleActorComponent::ReceiveHeal(UBattleActorComponent* Instigator, FGameplayTagContainer HealTags, float Value)
{
	FDamageHealTargetResult Result;
	Result.Target = this;

	if (!IsAlive())
	{
		return Result;
	}

	if (FBattleParameter* Health = Parameters.Find(HealthParameterTag))
	{
		const float Previous = Health->GetResourceValue();

		Value = ModifyHealingReceived(Value);
		Health->AddResourceValue(Value);

		const float NewValue = Health->GetResourceValue();
		const float AppliedHeal = Previous - NewValue;

		Result.AppliedValue = AppliedHeal;

		OnHealingReceived.Broadcast(this, Instigator, HealTags, AppliedHeal, Previous, NewValue);
	}

	return Result;
}

void UBattleActorComponent::SetIsAlive(bool bInIsAlive)
{
	if (bIsAlive == bInIsAlive) return;

	bIsAlive = bInIsAlive;
	if (bIsAlive)
	{
		if (FBattleParameter* Health = Parameters.Find(HealthParameterTag))
		{
			Health->AddResourceValue(Health->GetParameterValue());
		}

		OnRessurected.Broadcast(this);
	}
	else
	{
		if (FBattleParameter* Health = Parameters.Find(HealthParameterTag))
		{
			Health->RemoveResourceValue(Health->GetParameterValue());
		}

		OnKilled.Broadcast(this);
	}
}

void UBattleActorComponent::SetInCombat(bool bInIsInCombat)
{
	if (bIsInCombat == bInIsInCombat) return;

	bIsInCombat = bInIsInCombat;
	if (bIsInCombat) 
	{
		OnEnterCombat.Broadcast(this);
	}
	else
	{
		OnExitCombat.Broadcast(this);
	}
}

bool UBattleActorComponent::IsAlive() const
{
	return bIsAlive;
}

bool UBattleActorComponent::IsDead() const
{
	return !IsAlive();
}

bool UBattleActorComponent::IsInCombat() const
{
	return bIsInCombat;
}

ETeamType UBattleActorComponent::GetTeam() const
{
	return Team;
}

UBattleElement* UBattleActorComponent::GetBattleElementByIndex(int32 Index) const
{
	if (BattleElements.IsValidIndex(Index))
	{
		return BattleElements[Index];
	}

	return nullptr;
}

UBattleElement* UBattleActorComponent::GetBattleElementByData(UBattleElementData* ElementData) const
{
	if (ElementData && !BattleElements.IsEmpty())
	{
		for (UBattleElement* Element : GetBattleElements())
		{
			if (IsValid(Element) && Element->BattleElementData == ElementData) return Element;
		}
	}

	return nullptr;
}

UBattleEvent* UBattleActorComponent::GetBattleEventByClass(const UBattleElement* BattleElement, TSubclassOf<UBattleEvent> EventClass) const
{
	if (IsValid(BattleElement) && EventClass && !BattleElements.IsEmpty())
	{
		for (const UBattleElement* Element : GetBattleElements())
		{
			if (IsValid(Element) && Element == BattleElement)
			{
				const TArray<UBattleEvent*> Events = Element->GetBattleEvents();
				for (UBattleEvent* Event : Events)
				{
					if (IsValid(Event) && Event->IsA(EventClass)) return Event;
				}
			}
		}
	}

	return nullptr;
}

TArray<UBattleElement*> UBattleActorComponent::GetBattleElements() const
{
	return BattleElements;
}

TArray<UBattleElement*> UBattleActorComponent::GetBattleElementsInExecution() const
{
	TArray<UBattleElement*> ElementsInExecution;

	for (UBattleElement* Element : GetBattleElements())
	{
		if (IsValid(Element) && Element->bElementInExecution)
		{
			ElementsInExecution.Add(Element);
		}
	}
	return ElementsInExecution;
}

TArray<UBattleElement*> UBattleActorComponent::GetBattleElementsByDatas(const TArray<UBattleElementData*>& ElementDatas) const
{
	TArray<UBattleElement*> Result;

	if (BattleElements.IsEmpty() || ElementDatas.IsEmpty())
	{
		return Result;
	}

	TSet<UBattleElementData*> DataSet(ElementDatas);

	for (UBattleElement* Element : BattleElements)
	{
		if (!IsValid(Element)) continue;

		if (DataSet.Contains(Element->BattleElementData))
		{
			Result.Add(Element);
		}
	}

	return Result;
}

void UBattleActorComponent::InitializeParameter(UPARAM(ref)FBattleParameter& Parameter)
{
	if (Parameter.Data)
	{
		if (Parameter.Data->Event)
		{
			UBattleParameterEvent* NewEvent = DuplicateObject<UBattleParameterEvent>(Parameter.Data->Event, this);
			Parameter.Event = NewEvent;
		}

		Parameter.InitializeParameter();
	}
}

FBattleParameterModifier UBattleActorComponent::AddModifierToParameter(FGameplayTag ParameterTag, EParameterModifierType ModifierType, float ModifierValue)
{
	FBattleParameterModifier Modifier(ModifierType, ModifierValue);

	if (FBattleParameter* Parameter = Parameters.Find(ParameterTag))
	{
		Parameter->AddModifier(Modifier);

		if (Parameter->Event)
		{
			Parameter->Event->OnModifierAdded(Modifier);
		}
	}

	return Modifier;
}

void UBattleActorComponent::RemoveModifierFromParameter(FGameplayTag ParameterTag, FBattleParameterModifier Modifier)
{
	if (FBattleParameter* Parameter = Parameters.Find(ParameterTag))
	{
		Parameter->RemoveModifier(Modifier);

		if (Parameter->Event)
		{
			Parameter->Event->OnModifierRemoved(Modifier);
		}
	}
}

void UBattleActorComponent::IncreaseParameterResource(FGameplayTag ParameterTag, float Value)
{
	if (FBattleParameter* Parameter = Parameters.Find(ParameterTag))
	{
		const float Previous = Parameter->GetResourceValue();
		Parameter->AddResourceValue(Value);

		if (Parameter->Event)
		{
			Parameter->Event->OnResourceIncreased(Parameter->GetResourceValue(), Previous);
		}
	}
}

void UBattleActorComponent::DecreaseParameterResource(FGameplayTag ParameterTag, float Value)
{
	if (FBattleParameter* Parameter = Parameters.Find(ParameterTag))
	{
		const float Previous = Parameter->GetResourceValue();
		Parameter->RemoveResourceValue(Value);

		if (Parameter->Event)
		{
			Parameter->Event->OnResourceDecreased(Parameter->GetResourceValue(), Previous);
		}
	}
}

bool UBattleActorComponent::GetParameter(FGameplayTag ParameterTag, FBattleParameter& OutParam, float& OutParameterValue, float& OutResourceValue) const
{
	if (const FBattleParameter* Parameter = Parameters.Find(ParameterTag))
	{
		OutParam = *Parameter;
		OutParameterValue = Parameter->GetParameterValue();
		OutResourceValue = Parameter->GetResourceValue();

		return true;
	}

	return false;
}

bool UBattleActorComponent::GetHealthParameter(FBattleParameter& OutParam, float& OutParameterValue, float& OutResourceValue) const
{
	if (const FBattleParameter* Health = Parameters.Find(HealthParameterTag))
	{
		OutParam = *Health;
		OutParameterValue = Health->GetParameterValue();
		OutResourceValue = Health->GetResourceValue();

		return true;
	}

	return false;
}

float UBattleActorComponent::GetSimpleParameter(FGameplayTag ParameterTag, float Percentage) const
{
	if (const float* Parameter = SimpleParameters.Find(ParameterTag))
	{
		return *Parameter * Percentage;
	}

	return 0.f;
}

bool UBattleActorComponent::CanExecuteBattleElement_Implementation(UBattleElement* Element) const
{
	return true;
}

float UBattleActorComponent::ModifyDamageReceived_Implementation(float Damage)
{
	return Damage;
}

float UBattleActorComponent::ModifyHealingReceived_Implementation(float Healing)
{
	return Healing;
}


