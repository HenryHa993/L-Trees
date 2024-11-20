// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LSystemGeneratorSubsystem.h"
#include "GameFramework/Actor.h"
#include "LSystemGeneration.generated.h"

/*LSystemGeneration is responsible for drawing the tree.*/
UCLASS()
class FLECSTEST_API ALSystemGeneration : public AActor
{
	GENERATED_BODY()

public:
	ALSystemGeneration();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void Draw();

private:
	ULSystemGeneratorSubsystem* LSystemGenerator;
	FTransform OriginalTransform;
};
