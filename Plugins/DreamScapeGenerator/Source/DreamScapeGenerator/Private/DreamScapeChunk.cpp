// Fill out your copyright notice in the Description page of Project Settings.

#include "DreamScapeChunk.h"
#include "ProceduralMeshComponent.h"
#include "FastNoiseLite.h"
#include "Async/ParallelFor.h"

ADreamScapeChunk::ADreamScapeChunk()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UProceduralMeshComponent>("Chunk Mesh");
    Mesh->SetCastShadow(false);
	Mesh->bUseAsyncCooking = true;
	SetRootComponent(Mesh);

	HeightNoise = new FastNoiseLite();
	CaveNoise = new FastNoiseLite();
}

void ADreamScapeChunk::CreateHeightMap()
{
	Seed = FMath::Rand();
	auto SetupNoise = [&](FastNoiseLite* Noise, auto Type) {
		Noise->SetSeed(Seed);
		Noise->SetNoiseType(Type);
		Noise->SetFractalType(FastNoiseLite::FractalType_FBm);
		Noise->SetFrequency(Frequency);
		Noise->SetFractalLacunarity(Lacunarity);
		Noise->SetFractalOctaves(Octaves);
		Noise->SetFractalGain(Gain);
	};
	SetupNoise(HeightNoise, FastNoiseLite::NoiseType_Perlin);
	SetupNoise(CaveNoise, FastNoiseLite::NoiseType_Cellular);
}

void ADreamScapeChunk::Init(int32 InSize, int32 InBlockSize, FIntVector InCoord)
{
	ChunkSize = InSize;
	BlockSize = InBlockSize;
	ChunkCoord = InCoord;
	const int32 TotalBlocks = ChunkSize * ChunkSize * ChunkSize;
	Blocks.SetNumZeroed(TotalBlocks);
	CreateHeightMap();
	GenerateChunk();
}

void ADreamScapeChunk::GenerateChunk()
{
	GenerateBlocks();
	GenerateMesh();
}

void ADreamScapeChunk::RegenerateChunk()
{
	Mesh->ClearAllMeshSections();
	GenerateMesh();
}

bool ADreamScapeChunk::AddBlock(FIntVector Position, uint16 BlockID)
{
	bool bModified = ModifyBlock(Position, BlockID);
	if (bModified)
	{
		RegenerateChunk();
	}
	return bModified;
}

bool ADreamScapeChunk::RemoveBlock(FIntVector Position)
{
	bool bModified = ModifyBlock(Position, 0);
	if (bModified)
	{
		RegenerateChunk();
	}
	return bModified;
}

void ADreamScapeChunk::GenerateBlocks()
{
	const FVector Location = GetActorLocation() / BlockSize;
	ParallelFor(ChunkSize, [&](int x)
	{
			for (int y = 0; y < ChunkSize; y++)
			{
				const float Xpos = x + Location.X;
				const float Ypos = y + Location.Y;

				const int Height = FMath::Clamp(FMath::RoundToInt((HeightNoise->GetNoise(Xpos, Ypos) + 1) * ChunkSize / 2), 0, ChunkSize);

				for (int z = 0; z < ChunkSize; z++)
				{
					if (z < Height - 3) Blocks[Index(x, y, z)] = 1;
					else if (z < Height - 1) Blocks[Index(x, y, z)] = 1;
					else if (z == Height - 1) Blocks[Index(x, y, z)] = 1;
					else Blocks[Index(x, y, z)] = 0;
				}
			}
	});
}

void ADreamScapeChunk::GenerateMesh()
{
	VertexData.Reset();
	TriangleData.Reset();
	UVData.Reset();
	Colors.Reset();
	VertexCount = 0;
	GreedyMeshing();
	CreateMesh();
}

void ADreamScapeChunk::CreateMesh() const
{
	Mesh->SetMaterial(0, Material);
	Mesh->CreateMeshSection(0, VertexData, TriangleData, Normals, UVData, Colors, TArray<FProcMeshTangent>(), true);
}

bool ADreamScapeChunk::ModifyBlock(const FIntVector Location, uint16 NewBlock)
{
	if (!InBounds(Location.X, Location.Y, Location.Z)) return false;
	const int32 InIndex = Index(Location.X, Location.Y, Location.Z);
	Blocks[InIndex] = NewBlock;
	return true;
}

void ADreamScapeChunk::GreedyMeshing()
{
	for (int Axis = 0; Axis < 3; ++Axis)
	{
		// 2 Perpendicular axis
		const int Axis1 = (Axis + 1) % 3;
		const int Axis2 = (Axis + 2) % 3;
		const int MainAxisLimit = ChunkSize;
		const int Axis1Limit = ChunkSize;
		const int Axis2Limit = ChunkSize;
		auto DeltaAxis1 = FIntVector::ZeroValue;
		auto DeltaAxis2 = FIntVector::ZeroValue;
		auto ChunkItr = FIntVector::ZeroValue;
		auto AxisMask = FIntVector::ZeroValue;
		AxisMask[Axis] = 1;
		TArray<FMask> Mask;
		Mask.SetNum(Axis1Limit * Axis2Limit);
		// Check each slice of the chunk
		for (ChunkItr[Axis] = -1; ChunkItr[Axis] < MainAxisLimit;)
		{
			int N = 0;
			// Compute Mask
			for (ChunkItr[Axis2] = 0; ChunkItr[Axis2] < Axis2Limit; ++ChunkItr[Axis2])
			{
				for (ChunkItr[Axis1] = 0; ChunkItr[Axis1] < Axis1Limit; ++ChunkItr[Axis1])
				{
					const auto CurrentBlock = GetBlock(ChunkItr);
					const auto CompareBlock = GetBlock(ChunkItr + AxisMask);
					const bool CurrentBlockOpaque = CurrentBlock != 0;
					const bool CompareBlockOpaque = CompareBlock != 0;
					if (CurrentBlockOpaque == CompareBlockOpaque)
					{
						Mask[N++] = FMask{ 0, 0 };
					}
					else if (CurrentBlockOpaque)
					{
						Mask[N++] = FMask{ CurrentBlock, 1 };
					}
					else
					{
						Mask[N++] = FMask{ CompareBlock, -1 };
					}
				}
			}
			++ChunkItr[Axis];
			N = 0;
			// Generate Mesh From Mask
			for (int j = 0; j < Axis2Limit; ++j)
			{
				for (int i = 0; i < Axis1Limit;)
				{
					if (Mask[N].Normal != 0)
					{
						const auto CurrentMask = Mask[N];
						ChunkItr[Axis1] = i;
						ChunkItr[Axis2] = j;
						int Width;
						for (Width = 1; i + Width < Axis1Limit && CompareMask(Mask[N + Width], CurrentMask); ++Width)
						{
						}
						int Height;
						bool Done = false;
						for (Height = 1; j + Height < Axis2Limit; ++Height)
						{
							for (int k = 0; k < Width; ++k)
							{
								if (CompareMask(Mask[N + k + Height * Axis1Limit], CurrentMask)) continue;

								Done = true;
								break;
							}
							if (Done) break;
						}
						DeltaAxis1[Axis1] = Width;
						DeltaAxis2[Axis2] = Height;
						CreateQuad(
							CurrentMask, AxisMask, Width, Height,
							ChunkItr,
							ChunkItr + DeltaAxis1,
							ChunkItr + DeltaAxis2,
							ChunkItr + DeltaAxis1 + DeltaAxis2
						);
						DeltaAxis1 = FIntVector::ZeroValue;
						DeltaAxis2 = FIntVector::ZeroValue;
						for (int l = 0; l < Height; ++l)
						{
							for (int k = 0; k < Width; ++k)
							{
								Mask[N + k + l * Axis1Limit] = FMask{ 0, 0 };
							}
						}

						i += Width;
						N += Width;
					}
					else
					{
						i++;
						N++;
					}
				}
			}
		}
	}
}

void ADreamScapeChunk::CreateQuad(const FMask Mask,const FIntVector AxisMask,const int Width,const int Height,const FIntVector V1,const FIntVector V2,const FIntVector V3,const FIntVector V4)
{
	const auto Normal = FVector(AxisMask * Mask.Normal);
	const auto Color = FColor(0, 0, 0, GetTextureIndex(Mask.Block, Normal));

	VertexData.Append({
		FVector(V1) * 100,
		FVector(V2) * 100,
		FVector(V3) * 100,
		FVector(V4) * 100
		});

	TriangleData.Append({
		VertexCount,
		VertexCount + 2 + Mask.Normal,
		VertexCount + 2 - Mask.Normal,
		VertexCount + 3,
		VertexCount + 1 - Mask.Normal,
		VertexCount + 1 + Mask.Normal
		});

	Normals.Append({
		Normal,
		Normal,
		Normal,
		Normal
		});

	Colors.Append({
		Color,
		Color,
		Color,
		Color
		});

	if (Normal.X == 1 || Normal.X == -1)
	{
		UVData.Append({
			FVector2D(Width, Height),
			FVector2D(0, Height),
			FVector2D(Width, 0),
			FVector2D(0, 0),
			});
	}
	else
	{
		UVData.Append({
			FVector2D(Height, Width),
			FVector2D(Height, 0),
			FVector2D(0, Width),
			FVector2D(0, 0),
			});
	}

	VertexCount += 4;
}

//
// CHANGE LATER TO CORRECT TEXTURE INDEX FROM REGISTRY
//
uint8 ADreamScapeChunk::GetTextureIndex(uint16 BlockID, const FVector Normal) const
{
	if (Normal == FVector::UpVector || BlockID == 0) return 0;
	return 1;
}


