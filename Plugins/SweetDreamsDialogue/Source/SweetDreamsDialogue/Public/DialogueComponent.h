

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DialogueDelegates.h"
#include "DialogueComponent.generated.h"

class UDialogueData;
class USweetDreamsDialogueSubsystem;

// An option wrapper component that accesses and updates the Dialogue Subsystem.
UCLASS( ClassGroup=("SweetDreams"), meta=(BlueprintSpawnableComponent) )
class SWEETDREAMSDIALOGUE_API UDialogueComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UDialogueComponent();
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Dialogue")
	void StartDialogue(UDialogueData* Dialogue);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Dialogue")
	void UpdateDialogue();
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Dialogue")
	void SkipAnimatedDialogue();
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Dialogue")
	void SelectChoiceAndUpdate(FChoice Choice);

	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Dialogue")
	USweetDreamsDialogueSubsystem* GetDialogueSubsystem() const;

	UPROPERTY(BlueprintAssignable)
	FOnDialogueEvent OnDialogueStarted;
	UPROPERTY(BlueprintAssignable)
	FOnDialogueEvent OnDialogueEnded;
	UPROPERTY(BlueprintAssignable)
	FOnDialogueUpdated OnDialogueUpdated;
	UPROPERTY(BlueprintAssignable)
	FOnDialogueFunction OnDialogueCustomFunction;
	UPROPERTY(BlueprintAssignable)
	FOnDialogueAnimation OnDialogueAnimating;
	UPROPERTY(BlueprintAssignable)
	FOnDialogueAnimation OnDialogueAnimationFinished;
	UPROPERTY(BlueprintAssignable)
	FOnDialogueChoice OnDialogueChoices;
	UPROPERTY(BlueprintAssignable)
	FOnDialogueEvent OnDialogueNoChoices;
	UPROPERTY(BlueprintAssignable)
	FOnDialogueAudio OnDialogueAudio;
	UPROPERTY(BlueprintAssignable)
	FOnDialogueSequence OnDialogueSequence;
	UPROPERTY(BlueprintAssignable)
	FOnDialogueUpdated OnCustomDialogueMode;

protected:
	virtual void BeginPlay() override;

	void BindDelegates();

	UFUNCTION()
	void BroadcastDialogueStarted();
	UFUNCTION()
	void BroadcastDialogueEnded();
	UFUNCTION()
	void BroadcastDialogueUpdated(FSweetDreamsDialogue Dialogue, int32 Index);
};
