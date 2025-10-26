#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Data/BattleDataTypes.h"
#include "Battle/SweetDreamsBattleManager.h"
#include "SweetDreamsBattleInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class USweetDreamsBattleInterface : public UInterface
{
	GENERATED_BODY()
};

class SWEETDREAMSBATTLE_API ISweetDreamsBattleInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent, Category = "Sweet Dreams|Battle")
	void OnBattleStart(ASweetDreamsBattleManager* BattleManager);
	UFUNCTION(BlueprintImplementableEvent, Category = "Sweet Dreams|Battle")
	void OnBattleEnd(ASweetDreamsBattleManager* BattleManager, bool bVictorious);
	UFUNCTION(BlueprintImplementableEvent, Category = "Sweet Dreams|Battle")
	void OnBattleVictory(ASweetDreamsBattleManager* BattleManager);
	UFUNCTION(BlueprintImplementableEvent, Category = "Sweet Dreams|Battle")
	void OnBattleDefeat(ASweetDreamsBattleManager* BattleManager);

	UFUNCTION(BlueprintImplementableEvent, Category = "Sweet Dreams|Battle")
	void OnDamage(AActor* Target, float Amount);
	UFUNCTION(BlueprintImplementableEvent, Category = "Sweet Dreams|Battle")
	void OnReceiveDamage(AActor* Instigator, float Amount);
	UFUNCTION(BlueprintImplementableEvent, Category = "Sweet Dreams|Battle")
	void OnHeal(AActor* Target, float Amount);
	UFUNCTION(BlueprintImplementableEvent, Category = "Sweet Dreams|Battle")
	void OnReceiveHeal(AActor* Instigator, float Amount);
	UFUNCTION(BlueprintImplementableEvent, Category = "Sweet Dreams|Battle")
	void OnKill(AActor* Target);
	UFUNCTION(BlueprintImplementableEvent, Category = "Sweet Dreams|Battle")
	void OnReceiveKill(AActor* Instigator);
	UFUNCTION(BlueprintImplementableEvent, Category = "Sweet Dreams|Battle")
	void OnRevive(AActor* Target);
	UFUNCTION(BlueprintImplementableEvent, Category = "Sweet Dreams|Battle")
	void OnReceiveRevive(AActor* Instigator);
	UFUNCTION(BlueprintImplementableEvent, Category = "Sweet Dreams|Battle")
	void OnParameterUpdated(FBattleParamater& Parameter);

};

