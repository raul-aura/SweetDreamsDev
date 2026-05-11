
#include "LevelSystem/LevelCompletionComponent.h"
#include "LevelSystem/LevelManager.h"

ULevelCompletionComponent::ULevelCompletionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void ULevelCompletionComponent::BeginPlay()
{
	RegisterToLevelManager();

	Super::BeginPlay();
}

void ULevelCompletionComponent::RegisterToLevelManager()
{
	if (UGameInstance* GI = GetWorld()->GetGameInstance())
	{
		if (ULevelManager* LM = GI->GetSubsystem<ULevelManager>())
		{
			LM->RegisterExitComponent(this);
		}
	}
}

void ULevelCompletionComponent::ExitLevel()
{
	if (bCanExitLevel)
	{
		OnLevelExit.Broadcast();
	}
}

void ULevelCompletionComponent::ToggleCanExitLevel(bool bInCanExitLevel)
{
	bCanExitLevel = bInCanExitLevel;
}


