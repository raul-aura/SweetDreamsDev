// Fill out your copyright notice in the Description page of Project Settings.

#include "SweetDreamsTaskManager.h"
#include "Kismet/GameplayStatics.h"

ASweetDreamsTaskManager* ASweetDreamsTaskManager::GetTaskManager(const UObject* WorldContext)
{
	if (!ensureAlwaysMsgf(IsValid(WorldContext), TEXT("World Context was not valid.")))
	{
		return nullptr;
	}
	return Cast<ASweetDreamsTaskManager>(UGameplayStatics::GetActorOfClass(WorldContext, ASweetDreamsTaskManager::StaticClass()));
}

ASweetDreamsTask* ASweetDreamsTaskManager::FindTaskByName(const FName& Name) const
{
	if (Tasks.Num() == 0) return nullptr;
	for (ASweetDreamsTask* Task : Tasks)
	{
		if (IsValid(Task) && Task->GetTaskName().IsEqual(Name))
		{
			return Task;
		}
	}
	return nullptr;
}

ASweetDreamsTask* ASweetDreamsTaskManager::FindTaskByIndex(const int32& Index) const
{
	if (Tasks.Num() == 0) return nullptr;
	if (Tasks.IsValidIndex(Index))
	{
		return Tasks[Index];
	}
	return nullptr;
}

TArray<ASweetDreamsTask*> ASweetDreamsTaskManager::GetActiveTasks() const
{
	TArray<ASweetDreamsTask*> ActiveTasks = TArray<ASweetDreamsTask*>();
	if (Tasks.Num() == 0) return ActiveTasks;
	for (ASweetDreamsTask* Task : Tasks)
	{
		if (IsValid(Task) && Task->IsTaskActive())
		{
			ActiveTasks.Add(Task);
		}
	}
	return ActiveTasks;
}

void ASweetDreamsTask::StartTask()
{
	if (bIsActive) return;
	bIsActive = true;
	OnTaskStarted.Broadcast();
}

void ASweetDreamsTask::CompleteTask()
{
	if (!bIsActive) return;
	bIsActive = false;
	OnTaskCompleted.Broadcast();
}

void ASweetDreamsTask::EvaluateTaskProgress()
{
	if (Objectives.Num() == 0) return;
	for (FTaskObjective Objective : Objectives)
	{
		if (Objective.bIsCompleted == false)
		{
			return;
		}
	}
	CompleteTask();
}

void ASweetDreamsTask::CompleteTaskObjective(const FName Name)
{
	if (Objectives.Num() == 0 || Name.IsNone()) return;
	for (FTaskObjective& Objective : Objectives)
	{
		if (Objective.ObjectiveName.IsEqual(Name) && !Objective.bIsCompleted)
		{
			Objective.CompleteObjective();
			OnObjectiveCompleted.Broadcast(Objective.ObjectiveName);
		}
	}
}

void ASweetDreamsTask::CompleteTaskObjectiveIndex(const int32 Index)
{
	if (Objectives.Num() == 0 || !Objectives.IsValidIndex(Index) || Objectives[Index].bIsCompleted) return;
	Objectives[Index].CompleteObjective();
	OnObjectiveCompleted.Broadcast(Objectives[Index].ObjectiveName);
}

TArray<FTaskObjective> ASweetDreamsTask::GetTaskIncompleteObjectives() const
{
	TArray<FTaskObjective> Incomplete = TArray<FTaskObjective>();
	if (Objectives.Num() == 0) return Incomplete;
	for (FTaskObjective Objective : Objectives)
	{
		if (!Objective.bIsCompleted) Incomplete.Add(Objective);
	}
	return Incomplete;
}

int32 ASweetDreamsTask::GetCountOfCompletedObjectives() const
{
	if (Objectives.Num() == 0) return 0;
	int32 Completed = 0;
	for (FTaskObjective Objective : Objectives)
	{
		if (Objective.bIsCompleted) Completed++;
	}
	return Completed;
}

int32 ASweetDreamsTask::GetCountOfRemainingObjectives() const
{
	if (Objectives.Num() == 0) return 0;
	return GetTaskIncompleteObjectives().Num();
}
