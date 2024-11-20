#pragma once
#include "CoreMinimal.h"
/*#include "LSystemStructs.generated.h"

USTRUCT(BlueprintType)
struct FSymbols
{
	GENERATED_BODY()
	
	TCHAR Symbol;
};*/

UENUM(BlueprintType)
enum FunctionType
{
	Line,
	RotatePositive,
	RotateNegative,
	Cache,
	ReturnCache
};