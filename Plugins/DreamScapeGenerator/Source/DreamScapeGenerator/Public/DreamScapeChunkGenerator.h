// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DreamScapeChunkGenerator.generated.h"

class ADreamScapeChunk;

UCLASS()
class DREAMSCAPEGENERATOR_API ADreamScapeChunkGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	ADreamScapeChunkGenerator();

	UFUNCTION(BlueprintPure, Category = "Generator", meta = (WorldContext = "WorldContext", CallableWithoutWorldContext))
	static ADreamScapeChunkGenerator* GetDreamScapeChunkGenerator(UObject* WorldContext);

	UFUNCTION(BlueprintCallable, Category = "Generator")
	ADreamScapeChunk* GetChunk(const FVector& WorldLocation) const;
	UFUNCTION(BlueprintCallable, Category = "Generator")
	FIntVector GetChunkCoordinate(const FVector& WorldLocation) const;
	UFUNCTION(BlueprintCallable, Category = "Generator")
	FIntVector GetBlockCoordinate(const FVector& WorldLocation) const;
	UFUNCTION(BlueprintCallable, Category = "Generator")
	FIntVector GetBlockLocalLocation(const FVector& WorldLocation) const;

	UFUNCTION(BlueprintCallable, Category = "Generator")
	bool AddBlock(FVector Location, int32 BlockID);
	UFUNCTION(BlueprintCallable, Category = "Generator")
	bool RemoveBlock(FVector Location);

protected:
	virtual void BeginPlay() override;
	void GenerateChunks();

	UPROPERTY(EditAnywhere, Category = "Chunk")
	TSubclassOf<ADreamScapeChunk> ChunkClass;
	UPROPERTY(EditAnywhere, Category = "Chunk")
	int32 ChunkSize = 32;
	UPROPERTY(EditAnywhere, Category = "Chunk")
	int32 BlockSize = 100;
	UPROPERTY(EditAnywhere, Category = "Chunk")
	int32 GenerationRadius = 2;
	UPROPERTY(EditAnywhere, Category = "Chunk")
	bool bGenerateInZAxis = true;
	UPROPERTY(EditAnywhere, Category = "Chunk")
	bool bInvertZ = true;

	TMap<FIntVector, ADreamScapeChunk*> GeneratedChunks;
};
