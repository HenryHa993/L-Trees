// Fill out your copyright notice in the Description page of Project Settings.


#include "LSystemGeneration.h"


// Sets default values
ALSystemGeneration::ALSystemGeneration() :
	LSystemGenerator()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ALSystemGeneration::BeginPlay()
{
	Super::BeginPlay();

	UGameInstance* GameInstance = GetGameInstance();
	LSystemGenerator = GameInstance->GetSubsystem<ULSystemGeneratorSubsystem>();

	OriginalTransform = GetTransform();
	LSystemGenerator->OnGenerateDelegate.AddUniqueDynamic(this, &ALSystemGeneration::Draw);
}

// Called every frame
void ALSystemGeneration::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALSystemGeneration::Draw()
{
	//GetWorld()->debug
	SetActorTransform(OriginalTransform);
	LSystemGenerator->Draw(*this);
}
