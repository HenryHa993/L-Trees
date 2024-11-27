#pragma once
#include "CoreMinimal.h"

UENUM(BlueprintType)
enum FunctionType
{
	Line,
	RotatePositiveRoll,
	RotateNegativeRoll,
	Cache,
	ReturnCache,
	RotatePositiveYaw,
	RotateNegativeYaw,
	RotatePositivePitch,
	RotateNegativePitch
};