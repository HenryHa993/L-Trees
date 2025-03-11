// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LSystemStructs.h"
#include "LSystemGeneratorSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGenerateSignature);

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
	float LineThickness;
	float Angle;

	FString ClearLinesCommand = "FlushPersistentDebugLines";

	void DrawLine(AActor& Actor) const;
	void RotatePositiveRoll(AActor& Actor) const;
	void RotateNegativeRoll(AActor& Actor) const;
	void RotatePositiveYaw(AActor& Actor) const;
	void RotateNegativeYaw(AActor& Actor) const;
	void RotatePositivePitch(AActor& Actor) const;
	void RotateNegativePitch(AActor& Actor) const;

	void FlushLines() const;
	
public:
	UPROPERTY(BlueprintAssignable)
	FOnGenerateSignature OnGenerateDelegate;
	
	UFUNCTION(BlueprintCallable)
	void Generate();

	UFUNCTION(BlueprintCallable)
	void GenerateWithIterations(int Iterations);

	UFUNCTION(BlueprintCallable)
	void SetDefaults(FString UserAxiom, float UserAngle, float UserLineLength, float UserLineThickness);

	UFUNCTION(BlueprintCallable)
	bool AddRule(FString Input, FString Output);

	UFUNCTION(BlueprintCallable)
	bool AddFunction(FString Input, FunctionType TurtleFunction);

	UFUNCTION(BlueprintCallable)
	void ResetFunctions();

	UFUNCTION(BlueprintCallable)
	void ResetRules();
	
	void Draw(AActor& Actor);
};
