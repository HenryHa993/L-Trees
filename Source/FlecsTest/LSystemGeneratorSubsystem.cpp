// Fill out your copyright notice in the Description page of Project Settings.


#include "LSystemGeneratorSubsystem.h"

ULSystemGeneratorSubsystem::ULSystemGeneratorSubsystem() :
	Axiom("0"),
	CurrentString(Axiom),
	LineLength(50.0f),
	Angle(90.0f)
{
	
}

/*Initialise subsystem with some default values.*/
void ULSystemGeneratorSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	// TODO: Generate() should have a axiom passed in as well as an iterations number
	/*Axiom = "F";
	CurrentString = Axiom;
	
	Rules.Add('F', "F[+F]F[-F][F]");

	Angle = 25.7f;
	
	GenerateWithIterations(6);

	TurtleFunctions.Add('F', FunctionType::Line);
	TurtleFunctions.Add('+', FunctionType::RotatePositive);
	TurtleFunctions.Add('-', FunctionType::RotateNegative);
	TurtleFunctions.Add('[', FunctionType::Cache);
	TurtleFunctions.Add(']', FunctionType::ReturnCache);*/
	
	Super::Initialize(Collection);
}

void ULSystemGeneratorSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

/*Draw straight line with a specified length.*/
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
	
	DrawDebugLine(GetWorld(), StartPosition, EndPosition, FColor::Black, true, -1, 0, 5);
}

/*Apply positive rotation.*/
void ULSystemGeneratorSubsystem::RotatePositive(AActor& Actor)
{
	FRotator AppliedRotation(0,0,Angle);
	Actor.AddActorWorldRotation(AppliedRotation);
}

/*Apply negative rotation.*/
void ULSystemGeneratorSubsystem::RotateNegative(AActor& Actor)
{
	FRotator AppliedRotation(0,0,-Angle);
	Actor.AddActorWorldRotation(AppliedRotation);
}

/*Remove all lines. Used to re-draw trees.*/
void ULSystemGeneratorSubsystem::FlushLines()
{
	GetWorld()->Exec(GetWorld(), *ClearLinesCommand);
}

/*Single generation, carries on from previous iteration.*/
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

/*Generate with a specific set of iterations.*/
void ULSystemGeneratorSubsystem::GenerateWithIterations(int Iterations)
{
	for(int i = 0; i < Iterations; i++)
	{
		Generate();
	}
}

bool ULSystemGeneratorSubsystem::AddRule(FString Input, FString Output)
{
	/*if(Input.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Input empty."));
		return false;
	}
	if(1 < Input.Len())
	{
		UE_LOG(LogTemp, Warning, TEXT("Input too long."));
		return false;
	}
	if(Output.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Output empty.."));
		return false;
	}*/

	// Only accept single characters as variables.
	if(Input.IsEmpty() || 1 < Input.Len() || Output.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid Rule Insertion."));
		return false;
	}
	
	TArray<TCHAR> CharArray = Input.GetCharArray();

	Rules.Add(CharArray[0], Output);
	UE_LOG(LogTemp, Warning, TEXT("Added rule: %s -> %s"), *Input, *Output);
	return true;
}

/*Draw current generation.*/
// TODO: Animation mode using latent actions.
void ULSystemGeneratorSubsystem::Draw(AActor& Actor)
{
	FlushLines();

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
		default:
			break;
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
