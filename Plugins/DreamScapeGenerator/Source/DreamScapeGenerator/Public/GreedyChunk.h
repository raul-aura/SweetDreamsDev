// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GreedyChunk.generated.h"

class FastNoiseLite;
class UProceduralMeshComponent;

UCLASS()
class DREAMSCAPEGENERATOR_API AGreedyChunk : public AActor
{
	GENERATED_BODY()

	struct FMask
	{
		uint16 Block;
		int Normal;
	};
	
	AGreedyChunk();

public:
	virtual void Setup();
protected:
	virtual void Generate2DHeightMap(FVector Position);
	virtual void Generate3DHeightMap(FVector Position);
	virtual void GenerateMesh();
	virtual void ModifyVoxelData(const FIntVector Position, const uint16 Block);

private:
	UProceduralMeshComponent* Mesh;
	TArray<uint16> Blocks;
	int32 Size = 32;
	FastNoiseLite* Noise; 
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector2D> UV0;
	TArray<FVector> Normals;
	TArray<FColor> Colors;
	int32 VertexCount = 0;

	void CreateQuad(FMask Mask, FIntVector AxisMask, int Width, int Height, FIntVector V1, FIntVector V2, FIntVector V3, FIntVector V4);
	int GetBlockIndex(int X, int Y, int Z) const;
	uint16 GetBlock(FIntVector Index) const;
	bool CompareMask(FMask M1, FMask M2) const;
	int GetTextureIndex(uint16 Block, FVector Normal) const;
};


