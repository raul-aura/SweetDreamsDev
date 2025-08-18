// Fill out your copyright notice in the Description page of Project Settings.

#include "DreamScapeChunkGenerator.h"
#include "DreamScapeChunk.h"
#include "GreedyChunk.h"
#include "EngineUtils.h"

ADreamScapeChunkGenerator::ADreamScapeChunkGenerator()
{
	PrimaryActorTick.bCanEverTick = false;
	ChunkClass = ADreamScapeChunk::StaticClass();
}

ADreamScapeChunkGenerator* ADreamScapeChunkGenerator::GetDreamScapeChunkGenerator(UObject* WorldContext)
{
	if (!ensureAlwaysMsgf(IsValid(WorldContext), TEXT("World Context was not valid.")))
	{
		return nullptr;
	}
	if (UWorld* World = WorldContext->GetWorld())
	{
		for (TActorIterator<ADreamScapeChunkGenerator> It(World); It; ++It)
		{
			return *It;
		}
	}
	return nullptr;
}

void ADreamScapeChunkGenerator::BeginPlay()
{
	Super::BeginPlay();
	GenerateChunks();
}

void ADreamScapeChunkGenerator::GenerateChunks()
{
	for (int32 x = -GenerationRadius; x <= GenerationRadius; x++)
	{
		for (int32 y = -GenerationRadius; y <= GenerationRadius; y++)
		{
			if (bGenerateInZAxis)
			{
				for (int32 z = -GenerationRadius; z <= GenerationRadius; z++)
				{
					FIntVector Coord(x, y, z);
					const FVector Location = FVector(x * ChunkSize * BlockSize, y * ChunkSize * BlockSize, z * ChunkSize * BlockSize);
					ADreamScapeChunk* Chunk = GetWorld()->SpawnActor<ADreamScapeChunk>(Location, FRotator::ZeroRotator);
					if (IsValid(Chunk))
					{
						Chunk->Init(ChunkSize, BlockSize, Coord);
						GeneratedChunks.Add(Coord, Chunk);
					}
				}
			}
			else
			{
				FIntVector Coord(x, y, 0);
				const FVector Location = FVector(x * ChunkSize * BlockSize, y * ChunkSize * BlockSize, GetActorLocation().Z * ChunkSize * BlockSize);
				//ADreamScapeChunk* Chunk = GetWorld()->SpawnActor<ADreamScapeChunk>(Location, FRotator::ZeroRotator);
				AGreedyChunk* Chunk = GetWorld()->SpawnActor<AGreedyChunk>(Location, FRotator::ZeroRotator);
				if (IsValid(Chunk))
				{
					//Chunk->Init(ChunkSize, BlockSize, Coord);
					Chunk->Setup();
					//GeneratedChunks.Add(Coord, Chunk);
				}
			}
		}
	}
}

ADreamScapeChunk* ADreamScapeChunkGenerator::GetChunk(const FVector& WorldLocation) const
{
	const FIntVector Coord = GetChunkCoordinate(WorldLocation);
	if (GeneratedChunks.Contains(Coord))
	{
		return GeneratedChunks[Coord];
	}
	return nullptr;
}

FIntVector ADreamScapeChunkGenerator::GetChunkCoordinate(const FVector& WorldLocation) const
{
	const int32 X = FMath::FloorToInt(WorldLocation.X / (ChunkSize * BlockSize));
	const int32 Y = FMath::FloorToInt(WorldLocation.Y / (ChunkSize * BlockSize));
	const int32 Z = FMath::FloorToInt(WorldLocation.Z / (ChunkSize * BlockSize));
	return FIntVector(X, Y, Z);
}

FIntVector ADreamScapeChunkGenerator::GetBlockCoordinate(const FVector& WorldLocation) const
{
	return FIntVector(WorldLocation) / BlockSize;
}

FIntVector ADreamScapeChunkGenerator::GetBlockLocalLocation(const FVector& WorldLocation) const
{
	const FIntVector Coord = GetChunkCoordinate(WorldLocation);
	FIntVector LocalLocation = GetBlockCoordinate(WorldLocation) - Coord * ChunkSize;
	if (bInvertZ) LocalLocation.Z = ChunkSize - 1 - LocalLocation.Z;
	if (Coord.X < 0) LocalLocation.X--;
	if (Coord.Y < 0) LocalLocation.Y--;
	if (Coord.Z < 0) LocalLocation.Z--;
	return LocalLocation;
}

bool ADreamScapeChunkGenerator::AddBlock(FVector Location, int32 BlockID)
{
	ADreamScapeChunk* Chunk = GetChunk(Location);
	if (IsValid(Chunk))
	{
		FIntVector BlockLocation = GetBlockLocalLocation(Location);
		return Chunk->AddBlock(BlockLocation, BlockID);
	}
	return false;
}

bool ADreamScapeChunkGenerator::RemoveBlock(FVector Location)
{
	ADreamScapeChunk* Chunk = GetChunk(Location);
	if (IsValid(Chunk))
	{
		FIntVector BlockLocation = GetBlockLocalLocation(Location);
		return Chunk->RemoveBlock(BlockLocation);
	}
	return false;
}


