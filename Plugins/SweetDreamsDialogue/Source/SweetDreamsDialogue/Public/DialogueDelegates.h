#pragma once

#include "CoreMinimal.h"
//#include "UObject/Object.h"
#include "DialogueStructs.h"
#include "DialogueDelegates.generated.h"

class USoundBase;
class ULevelSequence;

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDialogueEvent);
UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDialogueUpdated, FSweetDreamsDialogue, Dialogue, int32, Index);
UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnDialogueFunction, FSweetDreamsDialogue, Dialogue, int32, Index, const TArray<FDialogueFunction>&, Functions);
UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnDialogueAnimation, FSweetDreamsDialogue, Dialogue, int32, Index, FText, AnimatedText);
UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnDialogueChoice, FSweetDreamsDialogue, Dialogue, int32, Index, const TArray<FChoice>&, Choices);
UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialogueAudio, USoundBase*, Audio);
UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnDialogueSequence, FSweetDreamsDialogue, Dialogue, int32, Index, ULevelSequence*, Sequence);

//UCLASS()
//class SWEETDREAMSDIALOGUE_API UDialogueDelegates : public UObject
//{
//    GENERATED_BODY()
//};