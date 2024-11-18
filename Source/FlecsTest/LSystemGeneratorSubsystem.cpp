// Fill out your copyright notice in the Description page of Project Settings.


#include "LSystemGeneratorSubsystem.h"

ULSystemGeneratorSubsystem::ULSystemGeneratorSubsystem() :
	Axiom("0"),
	CurrentString(Axiom),
	LineLength(50.0f),
	Angle(90.0f)
{
	
}

void ULSystemGeneratorSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	// TODO: Generate() should have a axiom passed in as well as an iterations number
	Axiom = "F";
	CurrentString = Axiom;
	
	Rules.Add('F', "F[+F]F[-F][F]");
	//Rules.Add('0', "1[0]0");

	Angle = 25.7f;
	
	Generate();
	Generate();
	Generate();
	Generate();
	Generate();

	TurtleFunctions.Add('F', FunctionType::Line);
	TurtleFunctions.Add('+', FunctionType::RotatePositive);
	TurtleFunctions.Add('-', FunctionType::RotateNegative);
	TurtleFunctions.Add('[', FunctionType::Cache);
	TurtleFunctions.Add(']', FunctionType::ReturnCache);
	
	Super::Initialize(Collection);
}

void ULSystemGeneratorSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void ULSystemGeneratorSubsystem::DrawLine(AActor& Actor)
{
	FTransform ActorTransform = Actor.GetTransform();
	
	FVector StartPosition = ActorTransform.GetLocation();
	//UE_LOG(LogTemp, Warning, TEXT("Start Position: %s"), *StartPosition.ToString());

	FVector UpVector = ActorTransform.GetRotation().GetUpVector();
	//UE_LOG(LogTemp, Warning, TEXT("Forward Direction: %s"), *ForwardVector.ToString());

	FVector EndPosition = StartPosition + (UpVector * LineLength);
	//UE_LOG(LogTemp, Warning, TEXT("End Position: %s"), *EndPosition.ToString());

	Actor.SetActorLocation(EndPosition);

	//GetWorld()->LineTraceSingleByChannel()
	
	DrawDebugLine(GetWorld(), StartPosition, EndPosition, FColor::Black, true, -1, 0, 5);
}

void ULSystemGeneratorSubsystem::RotatePositive(AActor& Actor)
{
	FRotator AppliedRotation(0,0,Angle);
	Actor.AddActorWorldRotation(AppliedRotation);
}

void ULSystemGeneratorSubsystem::RotateNegative(AActor& Actor)
{
	FRotator AppliedRotation(0,0,-Angle);
	Actor.AddActorWorldRotation(AppliedRotation);
}

void ULSystemGeneratorSubsystem::Generate()
{
	FString NewString = "";
	TArray<TCHAR> CharacterArray = CurrentString.GetCharArray();
	//UE_LOG(LogTemp, Warning, TEXT("Current string: %s"), *CurrentString);
	 
	for(int32 i = 0; i < CharacterArray.Num(); i++)
	{
		TCHAR CurrentCharacter = CharacterArray[i];
		if(Rules.Contains(CurrentCharacter))
		{
			NewString += Rules[CurrentCharacter];
			continue;
		}

		NewString += CurrentCharacter;
	}

	CurrentString = NewString;
	UE_LOG(LogTemp, Warning, TEXT("New string:%s"), *CurrentString);
}

void ULSystemGeneratorSubsystem::Draw(AActor& Actor)
{
	TArray<TCHAR> CharacterArray = CurrentString.GetCharArray();

	for(TCHAR CurrentCharacter : CharacterArray)
	{
		if(!TurtleFunctions.Contains(CurrentCharacter))
		{
			continue;
		}

		FunctionType TurtleFunction = TurtleFunctions[CurrentCharacter];

		switch (TurtleFunction)
		{
		case FunctionType::Line:
			DrawLine(Actor);
			break;
		case FunctionType::RotatePositive:
			RotatePositive(Actor);
			break;
		case FunctionType::RotateNegative:
			RotateNegative(Actor);
			break;
		case FunctionType::Cache:
			TransformStack.Push(Actor.GetTransform());
			break;
		case FunctionType::ReturnCache:
			Actor.SetActorTransform(TransformStack.Pop());
			break;
		}
	}
}
