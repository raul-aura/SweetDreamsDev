

#pragma once

#include "CoreMinimal.h"
#include "Save/SweetDreamsSaveFile.h"
#include "AuraSaveFile.generated.h"

UCLASS()
class AURASDK_API UAuraSaveFile : public USweetDreamsSaveFile
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, Category = "AuraSDK | Save")
	FName LastCheckpointID;
};
