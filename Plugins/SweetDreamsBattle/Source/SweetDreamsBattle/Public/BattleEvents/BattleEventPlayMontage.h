

#pragma once

#include "CoreMinimal.h"
#include "Data/BattleEvent.h"
#include "BattleEventPlayMontage.generated.h"

UCLASS()
class SWEETDREAMSBATTLE_API UBattleEventPlayMontage : public UBattleEvent
{
	GENERATED_BODY()

public:

	virtual void ExecuteEvent_Implementation() override;
	virtual bool IsFinished_Implementation() const override;

	UFUNCTION(BlueprintNativeEvent, Category = "Sweet Dreams|Battle|Battle Event Play Montage")
	float GetPlayRate() const;
	float GetPlayRate_Implementation() const;
	UFUNCTION(BlueprintNativeEvent, Category = "Sweet Dreams|Battle|Battle Event Play Montage")
	FName GetStartingSection() const;
	FName GetStartingSection_Implementation() const;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Play Montage")
	TObjectPtr<UAnimMontage> Montage = nullptr;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Play Montage")
	float PlayRate = 1.f;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Play Montage")
	FName StartingSection = NAME_None;

protected:

	UPROPERTY(BlueprintReadOnly, Category = "Play Montage")
	bool bMontageFinished = false;

	UPROPERTY(BlueprintReadOnly, Category = "Play Montage")
	int32 PendingMontages = 0;
};
