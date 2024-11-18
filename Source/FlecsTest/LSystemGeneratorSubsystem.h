// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LSystemStructs.h"
#include "LSystemGeneratorSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class FLECSTEST_API ULSystemGeneratorSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	ULSystemGeneratorSubsystem();
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

private:
	FString Axiom;
	FString CurrentString;
	
	TMap<TCHAR, FString> Rules;
	TMap<TCHAR, FunctionType> TurtleFunctions;

	TArray<FTransform> TransformStack;

	float LineLength;
	float Angle;

	void DrawLine(AActor& Actor);
	void RotatePositive(AActor& Actor);
	void RotateNegative(AActor& Actor);

public:
	UFUNCTION(BlueprintCallable)
	void Generate();

	void Draw(AActor& Actor);
};
