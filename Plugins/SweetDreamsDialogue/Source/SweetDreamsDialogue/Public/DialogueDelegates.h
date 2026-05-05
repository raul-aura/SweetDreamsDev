#pragma once

#include "CoreMinimal.h"
#include "DialogueStructs.h"
#include "DialogueDelegates.generated.h"

class USoundBase;
class ULevelSequence;
class UDialogueComponent;

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDialogueDelegate);

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialogueUpdated, FSweetDreamsDialogue, Dialogue);

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialogueAnimatedText, FText, AnimatedText);

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialogueChoice, const TArray<FChoice>&, Choices);

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialogueSequence, ULevelSequence*, Sequence);

