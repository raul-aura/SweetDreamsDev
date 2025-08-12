#pragma once

#include "CoreMinimal.h"
#include "DreamScapeEnums.generated.h"

UENUM(BlueprintType)
enum class EBlockDirection : uint8
{
	FORWARD,
	BACK,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

UENUM(BlueprintType)
enum class EBlockType : uint8
{
	NULL,
	AIR,
	WATER,
	DIRT,
	GRASS
};