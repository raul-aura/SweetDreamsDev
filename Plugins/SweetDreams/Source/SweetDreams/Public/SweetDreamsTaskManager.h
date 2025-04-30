// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Info.h"
#include "SweetDreamsTaskManager.generated.h"

UCLASS(Blueprintable)
class SWEETDREAMS_API ASweetDreamsTaskManager : public AInfo
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Sweet Dreams|Core|Tasks", meta = (WorldContext = "WorldContext", CallableWithoutWorldContext))
	static ASweetDreamsTaskManager* GetTaskManager(const UObject* WorldContext);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Tasks")
	ASweetDreamsTask* FindTaskByName(FName Name) const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Tasks")
	ASweetDreamsTask* FindTaskByIndex(int32 Index = 0) const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Tasks")
	ASweetDreamsTask* CompleteTaskByName(bool& bFoundTask, FName Name);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Tasks")
	ASweetDreamsTask* CompleteTaskByIndex(bool& bFoundTask, int32 Index = 0);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Tasks")
	TArray<ASweetDreamsTask*> GetActiveTasks() const;

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tasks")
	TArray<ASweetDreamsTask*> Tasks;
};

USTRUCT(BlueprintType)
struct FTaskObjective
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Task Objective")
	FName ObjectiveName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Task Objective")
	FText ObjectiveFriendlyName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Task Objective")
	bool bIsCompleted;

	FTaskObjective() : ObjectiveName("Objective"), ObjectiveFriendlyName(NSLOCTEXT("Task", "ObjectiveName", "My Objective")), bIsCompleted(false) {}

	FTaskObjective(FName InObjectiveName, FText InObjectiveFriendlyName)
		: ObjectiveName(InObjectiveName)
		, ObjectiveFriendlyName(InObjectiveFriendlyName)
		, bIsCompleted(false)
	{}

	void CompleteObjective()
	{
		bIsCompleted = true;
	}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTaskEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTaskObjectiveEvent, const FName&, ObjectiveName);

UCLASS(Blueprintable)
class SWEETDREAMS_API ASweetDreamsTask : public AActor
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Task")
	void StartTask();
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Task")
	void CompleteTask();
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Task")
	void EvaluateTaskProgress();
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Task")
	void CompleteTaskObjective(const FName Name);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Task")
	void CompleteTaskObjectiveIndex(const int32 Index);

	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Task")
	FName GetTaskName() const { return TaskName; }
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Task")
	TArray<FTaskObjective> GetTaskObjectives() const { return Objectives; }
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Task")
	TArray<FTaskObjective> GetTaskIncompleteObjectives() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Task")
	int32 GetCountOfCompletedObjectives() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Task")
	int32 GetCountOfRemainingObjectives() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Task")
	bool IsTaskActive() const { return bIsActive; }
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Task")
	bool IsTaskComplete() const { return bIsComplete; }

	UPROPERTY(BlueprintAssignable, Category = "Sweet Dreams|Core|Task")
	FTaskEvent OnTaskStarted;
	UPROPERTY(BlueprintAssignable, Category = "Sweet Dreams|Core|Task")
	FTaskEvent OnTaskCompleted;
	UPROPERTY(BlueprintAssignable, Category = "Sweet Dreams|Core|Task")
	FTaskObjectiveEvent OnObjectiveCompleted;

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Task")
	FName TaskName = "Task";
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Task")
	FText TaskFriendlyName = NSLOCTEXT("Task", "TaskFriendlyName", "My Task");
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Task")
	FText TaskDescription = NSLOCTEXT("Task", "TaskDescription", "A task for your game.");
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Task")
	bool bIsActive = false;
	UPROPERTY(BlueprintReadWrite, Category = "Task")
	bool bIsComplete = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Task")
	TArray<FTaskObjective> Objectives;
};