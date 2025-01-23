// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleItem.h"
#include "BattlerDataComponent.h"
#include "BattleAction.h"

void USweetDreamsItemRPG::OnCreateItem_Implementation()
{
    if (IsValid(LevelClass))
    {
        LevelObject = NewObject<USweetDreamsLevel>(this, LevelClass);
        if (IsValid(LevelObject))
        {
            LevelObject->OnLevelUpgraded.AddDynamic(this, &USweetDreamsItemRPG::UpgradeItem);
        }
    }
}

void USweetDreamsItemRPG::OnEquipItem_Implementation()
{
    UpdateParams();
    if (bApplyParamsOnAdd) ApplyParams();
}

void USweetDreamsItemRPG::OnUnequipItem_Implementation()
{
    UpdateParams();
    if (bRemoveParamsOnRemove) RemoveParams();
}

void USweetDreamsItemRPG::ApplyParams()
{
    if (IsValid(GetItemOwner()))
    {
        UBattlerDataComponent* Data = UBattlerDataComponent::GetBattlerDataComponent(GetItemOwner());
        if (IsValid(Data))
        {
            Data->UpdateHealthValue(Health, HealthPercentage);
            Data->UpdateManaValue(Mana, ManaPercentage);
            Data->UpdateForceValue(Force, ForcePercentage);
            Data->UpdateForceMultiplier(ForceMultiplier);
            Data->UpdateResistenceValue(Resistence, ResistencePercentage);
            Data->UpdateResistenceMultiplier(ResistenceMultiplier);
            Data->UpdateSpeedValue(Speed, SpeedPercentage);
            Data->UpdateSpeedMultiplier(SpeedMultiplier);
            Data->UpdateDamageDealtMultiplier(DamageDealtMultiplier);
            Data->UpdateDamageReceivedMultiplier(DamageReceivedMultiplier);
            Data->UpdateHealMultiplier(HealMultiplier);
            Data->UpdateManaRestoreMultiplier(ManaRestoreMultiplier);
            Data->UpdateAdditionalActions(Actions);
            Data->UpdateAdditionalLives(Lives);
            Data->LearnActions(ActionClasses);
            Data->AddStates(StateClasses, this);
        }
    }
}

void USweetDreamsItemRPG::RemoveParams()
{
    if (IsValid(GetItemOwner()))
    {
        UBattlerDataComponent* Data = UBattlerDataComponent::GetBattlerDataComponent(GetItemOwner());
        if (IsValid(Data))
        {
            Data->UpdateHealthValue(-Health, -HealthPercentage);
            Data->UpdateManaValue(-Mana, -ManaPercentage);
            Data->UpdateForceValue(-Force, -ForcePercentage);
            Data->UpdateForceMultiplier(-ForceMultiplier);
            Data->UpdateResistenceValue(-Resistence, -ResistencePercentage);
            Data->UpdateResistenceMultiplier(-ResistenceMultiplier);
            Data->UpdateSpeedValue(-Speed, -SpeedPercentage);
            Data->UpdateSpeedMultiplier(-SpeedMultiplier);
            Data->UpdateDamageDealtMultiplier(-DamageDealtMultiplier);
            Data->UpdateDamageReceivedMultiplier(-DamageReceivedMultiplier);
            Data->UpdateHealMultiplier(-HealMultiplier);
            Data->UpdateManaRestoreMultiplier(-ManaRestoreMultiplier);
            Data->UpdateAdditionalActions(-Actions);
            Data->UpdateAdditionalLives(-Lives);
            Data->RemoveActions(ActionClasses);
            Data->RemoveStates(StateClasses);
        }
    }
}

void USweetDreamsItemRPG::UpgradeItem()
{
    RemoveParams();
    UpdateParams();
    ApplyParams();
}

void USweetDreamsItemRPG::UpdateParams()
{
    UpdateParameterByLevel(Health, BaseHealth, AdditionalHealth);
    UpdateParameterByLevel(HealthPercentage, BaseHealthPercentage, AdditionalHealthPercentage);
    UpdateParameterByLevel(Mana, BaseMana, AdditionalMana);
    UpdateParameterByLevel(ManaPercentage, BaseManaPercentage, AdditionalManaPercentage);
    UpdateParameterByLevel(Force, BaseForce, AdditionalForce);
    UpdateParameterByLevel(ForcePercentage, BaseForcePercentage, AdditionalForcePercentage);
    UpdateParameterByLevel(Resistence, BaseResistence, AdditionalResistence);
    UpdateParameterByLevel(ResistencePercentage, BaseResistencePercentage, AdditionalResistencePercentage);
    float SpeedF = Speed;
    Speed = UpdateParameterByLevel(SpeedF, BaseSpeed, AdditionalSpeed);
    UpdateParameterByLevel(SpeedPercentage, BaseSpeedPercentage, AdditionalSpeedPercentage);
    float ActionsF = Actions;
    Actions = UpdateParameterByLevel(ActionsF, BaseActions, AdditionalActions);
    float LivesF = Lives;
    Lives = UpdateParameterByLevel(LivesF, BaseLives, AdditionalLives);
}

float USweetDreamsItemRPG::UpdateParameterByLevel(float& Parameter, float BaseParameter, float AdditionalParameter, float QuadraticFactor)
{
    if (!IsValid(LevelObject)) return 0.f;
    Parameter = BaseParameter + (AdditionalParameter * (LevelObject->GetCurrentLevel() - 1)) + (QuadraticFactor * FMath::Square(LevelObject->GetCurrentLevel() - 1));
    return Parameter;
}
