
#include "Components/CheckpointComponent.h"
#include "Core/SweetDreamsBPLibrary.h"
#include "Save/AuraSaveFile.h"
#include "EngineUtils.h"

UCheckpointComponent::UCheckpointComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

UCheckpointComponent* UCheckpointComponent::FindCheckpointByID(const UObject* WorldContextObject, FName Identifier)
{
	if (!IsValid(WorldContextObject) || !Identifier.IsValid()) return nullptr;

	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);

	for (TActorIterator<AActor> It(World); It; ++It)
	{
		if (AActor* Actor = *It)
		{
			if (UCheckpointComponent* Comp = Actor->FindComponentByClass<UCheckpointComponent>())
			{
				if (Comp->CheckpointID == Identifier)
				{
					return Comp;
				}
			}
		}
	}

	return nullptr;
}

void UCheckpointComponent::UpdateCheckpointRespawn(FVector Location, FRotator Rotation)
{
	RespawnLocation = Location;
	RespawnRotation = Rotation;
}

void UCheckpointComponent::CheckpointSaveGame(UPARAM(ref) bool& bExecutedSaveOperation)
{
	if (!bExecutedSaveOperation)
	{
		bExecutedSaveOperation = true;
		OnCheckpointFirstInteracted.Broadcast();
	}

	if (UAuraSaveFile* Save = Cast<UAuraSaveFile>(USweetDreamsBPLibrary::GetPersistentSave(this)))
	{
		Save->LastCheckpointID = CheckpointID;
	}

	bool bSuccess = USweetDreamsBPLibrary::SavePersistentGame(this);
	if (!bSuccess)
	{
		USweetDreamsBPLibrary::CreatePersistentSave(this, CustomSaveClass, bSuccess);
	}

	if (bSuccess)
	{
		OnGameSaved.Broadcast();
	}
}

void UCheckpointComponent::BeginPlay()
{
	Super::BeginPlay();
}



