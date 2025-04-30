// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BattleElement.generated.h"

UENUM(BlueprintType)
enum class ETargetType : uint8
{
	Ally UMETA(DisplayName = "Allies"),
	AllAlly UMETA(DisplayName = "All Allies"),
	DeadAlly UMETA(DisplayName = "Dead Allies"),
	Enemy UMETA(DisplayName = "Enemies Only"),
	AllEnemy UMETA(DisplayName = "All Enemies"),
	Self UMETA(DisplayName = "Self Only"),
};

UENUM(BlueprintType)
enum class EStateMatchCondition : uint8
{
	AnyMatch   UMETA(DisplayName = "OR (Any Match)"),
	AllMatch   UMETA(DisplayName = "AND (All Must Match)")
};

UCLASS(Blueprintable, BlueprintType)
class SWEETDREAMSBATTLE_API UBattleElement : public UObject
{
	GENERATED_BODY()
	
public: 

	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Element")
	virtual void SetOwner(AActor* InputOwner);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Element")
	virtual AActor* GetOwner() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Element")
	virtual ABattleCharacter* GetBattleOwner() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Element")
	virtual TArray<AActor*> GetOwnerAsArray() const;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Sweet Dreams|RPG|Element")
	FText GetElementName() const;
	virtual FText GetElementName_Implementation() const { return ElementName; }
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Sweet Dreams|RPG|Element")
	FText GetElementDescription() const;
	virtual FText GetElementDescription_Implementation() const { return ElementDescription; }
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Sweet Dreams|RPG|Element")
	FText GetElementMessage() const;
	virtual FText GetElementMessage_Implementation() const { return ElementMessage; }
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Element")
	virtual void ShowMessage();
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Element")
	virtual void UpdateElementDescription(FText NewDescription);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Element")
	virtual bool IsElementHidden() const { return bIsElementHidden; }
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Element")
	virtual void SetElementHidden(bool bIsHidden = true);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Element")
	virtual void SetCurrentBattle(ASweetDreamsBattleManager* Battle);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Element")
	virtual void AddTarget(AActor* Target, bool bRemoveDead = true);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Element")
	virtual void RemoveTarget(AActor* Target);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Element")
	virtual void SetTarget(TArray<AActor*> NewTargets, bool bRemoveDead = true);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Element")
	virtual void SetTargetRandom(TArray<AActor*> PossibleTargets, int32 TargetAmount, bool bRemoveDead = true);
	UFUNCTION(BlueprintPure, Category = "Sweet Dreams|RPG|Element")
	virtual TArray<AActor*> GetAdjacentTargets(AActor* PrimaryTarget, const TArray<AActor*>& TargetsToSearch) const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Element")
	virtual void AddAdjacentTargets(AActor* PrimaryTarget, const TArray<AActor*>& TargetsToSearch);
	UFUNCTION(BlueprintPure, Category = "Sweet Dreams|RPG|Element")
	virtual AActor* GetFirstElementTarget() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Element")
	virtual bool UpdateValidTargets();
	//
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Element", meta = (ReturnDisplayName = "All Targets Killed"))
	virtual bool DamageTargets(TArray<AActor*> Targets, float& PostMitigatedDamage, int32& KilledTargets, float Damage = 100.0f, float ResistenceShred = 0.f, bool bCanBeMitigated = true, bool bApplyCalculations = true, bool bIsAdditionalDamage = false);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Element")
	virtual void HealTargets(TArray<AActor*> Targets, float& HealedAmount, float& OverhealAmount, float Heal = 100.0f);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Element")
	virtual void RestoreManaTargets(TArray<AActor*> Targets, float& RestoredAmount, float& OverflowAmount, float Restore = 100.0f);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Element", meta = (ReturnDisplayName = "All States Added"))
	static bool AddStatesToTargets(UObject* StateInstigator, TArray<TSubclassOf<UBattleState>> States, TArray<AActor*> Targets, int32& StatesAdded, float Chance = 1.0f);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Element", meta = (ReturnDisplayName = "All States Removed"))
	static bool RemoveStatesOfTargets(TArray<TSubclassOf<UBattleState>> States, TArray<AActor*> Targets, int32& StatesRemoved, float Chance = 1.0f);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Element")
	static void CleanseTargets(TArray<AActor*> Targets, int32& StatesRemoved);
	UFUNCTION(BlueprintPure, Category = "Sweet Dreams|RPG|Element")
	static bool DoesTargetsHasStates(TArray<AActor*> Targets, TArray<TSubclassOf<UBattleState>> States, EStateMatchCondition MatchCondition);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Element")
	virtual void KillTargets(TArray<AActor*> Targets);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Element")
	static void ReviveTargets(TArray<AActor*> Targets, float HealthRestore = 100.f, float ManaRestore = 100.f);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Element")
	static void SetAbilityToAct(TArray<AActor*> Targets, bool bIsAbleToAct = false);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Element", meta = (ReturnDisplayName = "Animation Length"))
	virtual float StartAnimation(UAnimSequence* Animation, TArray<AActor*> Targets);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Element")
	virtual void CreateAndForceAction(TSubclassOf<UBattleAction> Action, TArray<AActor*> Targets, bool bUseCooldown = true, int32 OverrideSpeed = -1);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Element")
	virtual void TriggerSound(USoundBase* Sound, float Volume = 1.f, float Pitch = 1.f, float Delay = 0.f);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Element")
	virtual void TriggerSoundAtLocation(USoundBase* Sound, FVector Location, float Volume = 1.f, float Pitch = 1.f, float Delay = 0.f);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Element")
	virtual class ASweetDreamsDialogueManager* StartDialogue(FName DialogueName, float StartTransition = 2.0f);
	// events
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Sweet Dreams|RPG|Element")
	void OnBattleStart(ASweetDreamsBattleManager* BattleReference);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Sweet Dreams|RPG|Element")
	void OnBattleEnd(ASweetDreamsBattleManager* BattleReference);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Sweet Dreams|RPG|Element")
	void OnTurnStart(int32 Turn = 0);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Sweet Dreams|RPG|Element")
	void OnOwnerActionEnd(UBattleAction* Action);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Sweet Dreams|RPG|Element")
	void OnActionCountIncremented(int32 ActionCount);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Sweet Dreams|RPG|Element")
	void OnOwnerTick(float DeltaTime);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Sweet Dreams|RPG|Element")
	void OnManaRestored(float RestoredAmount);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Sweet Dreams|RPG|Element")
	void OnManaConsumed(float ConsumedAmount);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Sweet Dreams|RPG|Element")
	void OnManaRestoreGranted(const TArray<AActor*>& Targets, float RestoredAmount, float OverflowAmount);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Sweet Dreams|RPG|Element")
	void OnHealed(float HealedAmount);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Sweet Dreams|RPG|Element")
	void OnHealingGranted(const TArray<AActor*>& Targets, float HealedAmount, float OverhealAmount);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Sweet Dreams|RPG|Element")
	void OnDamageDealt(const TArray<AActor*>& Targets, float DamageAmount, bool bKilledTarget);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Sweet Dreams|RPG|Element")
	float OnPreDamageReceived(float DamageAmount, AActor* DamageInstigator);
	float OnPreDamageReceived_Implementation(float DamageAmount, AActor* DamageInstigator);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Sweet Dreams|RPG|Element")
	void OnPostDamageReceived(float DamageAmount, AActor* DamageInstigator, bool bIsAdditionalDamage);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Sweet Dreams|RPG|Element")
	void OnKilled(AActor* KillInstigator, int32 CurrentLives);

protected:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Sweet Dreams|RPG|Element")
	void OnDialogueStarted();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Sweet Dreams|RPG|Element")
	void OnDialogueEnded();
	// ELEMENT
	UPROPERTY(BlueprintReadOnly, Category = "Battle Element")
	AActor* Owner;
	UPROPERTY(BlueprintReadOnly, Category = "Battle Element")
	ASweetDreamsBattleManager* CurrentBattle;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Battle Element")
	FText ElementName = FText::FromString(TEXT("Element"));
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Battle Element", meta = (MultiLine = true))
	FText ElementDescription = FText::FromString(TEXT("Write about your element here..."));
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Battle Element", meta = (MultiLine = true))
	FText ElementMessage = FText::FromString(TEXT("This element was activated!"));
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Battle Element")
	TArray<AActor*> ElementTargets;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Battle Element")
	bool bIsElementHidden = false;
	//
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Element", meta = (ExpandBoolAsExecs = "ReturnValue"))
	static bool AreTargetsValid(const TArray<AActor*>& Targets);
};
