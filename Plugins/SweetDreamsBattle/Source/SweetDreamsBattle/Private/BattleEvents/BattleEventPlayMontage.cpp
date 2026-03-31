

#include "BattleEvents/BattleEventPlayMontage.h"
#include "Battle/BattleActorComponent.h"
#include "GameFramework/Character.h"

void UBattleEventPlayMontage::ExecuteEvent_Implementation()
{
    if (!BattleContext) return;

    PendingMontages = 0;
    bMontageFinished = false;

    for (const UBattleActorComponent* Target : BattleContext->GetSelectedTargets(TargetSettings))
    {
        if (!IsValid(Target))
        {
            continue;
        }

        AActor* Owner = Target->GetOwner();

        if (!IsValid(Owner))
        {
            continue;
        }

        if (ACharacter* Character = Cast<ACharacter>(Owner))
        {
            if (UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance())
            {
                if (Montage)
                {
                    AnimInstance->Montage_Play(Montage, GetPlayRate());

                    const FName Section = GetStartingSection();
                    if (!Section.IsNone())
                    {
                        AnimInstance->Montage_JumpToSection(Section, Montage);
                    }

                    PendingMontages++;

                    FOnMontageEnded EndDelegate;
                    EndDelegate.BindWeakLambda(this,
                        [this](UAnimMontage* EndedMontage, bool bInterrupted)
                        {
                            PendingMontages--;

                            if (PendingMontages <= 0)
                            {
                                bMontageFinished = true;
                            }
                        }
                    );

                    AnimInstance->Montage_SetEndDelegate(EndDelegate, Montage);
                }
            }
        }
    }
}

bool UBattleEventPlayMontage::IsFinished_Implementation() const
{
	return bMontageFinished;
}

float UBattleEventPlayMontage::GetPlayRate_Implementation() const
{
	return PlayRate;
}

FName UBattleEventPlayMontage::GetStartingSection_Implementation() const
{
	return StartingSection;
}
