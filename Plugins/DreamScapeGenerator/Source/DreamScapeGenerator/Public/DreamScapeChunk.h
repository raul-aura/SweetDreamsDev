// Fill out your copyright notice in the Description page of Project Settings.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DreamScapeChunk.generated.h"

class FastNoiseLite;
class UProceduralMeshComponent;

UCLASS()
class DREAMSCAPEGENERATOR_API ADreamScapeChunk : public AActor
{
	GENERATED_BODY()

	ADreamScapeChunk();

	struct FMask
	{
		uint16 Block;
		int Normal;
	};

public:
	// chunk
	void Init(int32 InSize = 32, int32 InBlockSize = 100, FIntVector InCoord = FIntVector::ZeroValue);
	void GenerateChunk();
	void RegenerateChunk();

	// edit
	bool AddBlock(FIntVector Position, uint16 BlockID = 1);
	bool RemoveBlock(FIntVector Position);

private:
	// components
	UProceduralMeshComponent* Mesh;
	FastNoiseLite* HeightNoise;
	FastNoiseLite* CaveNoise;

	// chunk
	UMaterial* Material;
	int32 ChunkSize = 32;
	int32 ChunkScale = 1;
	int32 BlockSize = 100;
	FIntVector ChunkCoord = FIntVector::ZeroValue;
	TArray<uint16> Blocks;
	TArray<FVector> VertexData;
	TArray<int32> TriangleData;
	TArray<FVector2D> UVData;
	TArray<FVector> Normals;
	TArray<FColor> Colors;
	int32 VertexCount = 0;

	// noise
	int32 Seed;
	float Frequency = 0.03f;
	int32 Octaves = 3;
	float Lacunarity = 2.0f;
	float Gain = 0.4f;

	// funcs
	void CreateHeightMap();
	void GenerateBlocks(); // setting which block will have in each coord according to the height/cave maps
	void GenerateMesh();
	void CreateMesh() const;
	bool ModifyBlock(const FIntVector Location, uint16 NewBlock);

	// Greedy mesh
	void GreedyMeshing();
	void CreateQuad(const FMask Mask, const FIntVector AxisMask, const int Width, const int Height, const FIntVector V1, const FIntVector V2, const FIntVector V3, const FIntVector V4);
	bool CompareMask(FMask M1, FMask M2) const { return M1.Block == M2.Block && M1.Normal == M2.Normal; }

	// Material helpers
	uint8 GetTextureIndex(uint16 BlockID, const FVector Normal) const;

	FORCEINLINE int32 Index(int32 X, int32 Y, int32 Z) const
	{
		return Z * ChunkSize * ChunkSize + Y * ChunkSize + X;
	}

	bool InBounds(int32 X, int32 Y, int32 Z) const
	{
		return (X >= 0 && X < ChunkSize &&
			Y >= 0 && Y < ChunkSize &&
			Z >= 0 && Z < ChunkSize);
	}

	uint16 GetBlock(int32 X, int32 Y, int32 Z) const
	{
		return InBounds(X, Y, Z) ? Blocks[Index(X, Y, Z)] : 0;
	}

	uint16 GetBlock(const FIntVector Location) const
	{
		return GetBlock(Location.X, Location.Y, Location.Z);
	}

	bool IsAir(uint16 BlockID) const
	{
		return BlockID == 0;
	}
};

