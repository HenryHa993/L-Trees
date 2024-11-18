#pragma once
#include "CoreMinimal.h"
/*#include "LSystemStructs.generated.h"

USTRUCT(BlueprintType)
struct FSymbols
{
	GENERATED_BODY()
	
	TCHAR Symbol;
};*/

enum class FunctionType
{
	Line,
	RotatePositive,
	RotateNegative,
	Cache,
	ReturnCache
};