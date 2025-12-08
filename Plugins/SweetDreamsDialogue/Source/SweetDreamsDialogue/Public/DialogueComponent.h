

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DialogueComponent.generated.h"


UCLASS( ClassGroup=("SweetDreams"), meta=(BlueprintSpawnableComponent) )
class SWEETDREAMSDIALOGUE_API UDialogueComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UDialogueComponent();

protected:
	virtual void BeginPlay() override;

		
	
};
