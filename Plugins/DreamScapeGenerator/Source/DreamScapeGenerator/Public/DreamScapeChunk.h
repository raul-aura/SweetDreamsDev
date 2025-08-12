#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DreamScapeChunk.generated.h"

enum class EBlockDirection;
enum class EBlockType;

UCLASS()
class DREAMSCAPEGENERATOR_API ADreamScapeChunk : public AActor
{
	GENERATED_BODY()

private:

	const FVector BlockVertexData[8] = {
 FVector(100,100,100),
 FVector(100,0,100),
 FVector(100,0,0),
 FVector(100,100,0),
 FVector(0,0,100),
 FVector(0,100,100),
 FVector(0,100,0),
 FVector(0,0,0)
	};

	const int BlockTriangleData[24] = {
	 0,1,2,3, // Forward
	 5,0,3,6, // Right
	 4,5,6,7, // Back
	 1,4,7,2, // Left
	 5,4,1,0, // Up
	 3,2,7,6  // Down
	};
};

