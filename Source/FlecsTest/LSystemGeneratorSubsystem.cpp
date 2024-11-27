// Fill out your copyright notice in the Description page of Project Settings.


#include "LSystemGeneratorSubsystem.h"

#include "LSystemGeneration.h"
#include "Kismet/GameplayStatics.h"

ULSystemGeneratorSubsystem::ULSystemGeneratorSubsystem() :
	Axiom("0"),
	CurrentString(Axiom),
	LineLength(50.0f),
	LineThickness(3.0f),
	Angle(90.0f)
{
	
}

/*Initialise subsystem with some default values.*/
void ULSystemGeneratorSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
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
	/*FVector UpVector = ActorTransform.GetRotation().GetUpVector();
	FVector EndPosition = StartPosition + (UpVector * LineLength);*/
	FVector UpVector = ActorTransform.GetRotation().GetForwardVector();
	FVector EndPosition = StartPosition + (UpVector * LineLength);

	Actor.SetActorLocation(EndPosition);
	
	DrawDebugLine(GetWorld(), StartPosition, EndPosition, FColor::Black, true, -1, 0, LineThickness);
}

// X
/*Apply positive rotation.*/
void ULSystemGeneratorSubsystem::RotatePositiveRoll(AActor& Actor)
{
	FRotator AppliedRotation(0,0,Angle);
	//FRotator AppliedRotation(0,Angle,0);
	//Actor.AddActorWorldRotation(AppliedRotation);
	Actor.AddActorLocalRotation(AppliedRotation);
}

/*Apply negative rotation.*/
void ULSystemGeneratorSubsystem::RotateNegativeRoll(AActor& Actor)
{
	FRotator AppliedRotation(0,0,-Angle);
	//FRotator AppliedRotation(0,-Angle,0);
	//Actor.AddActorWorldRotation(AppliedRotation);
	Actor.AddActorLocalRotation(AppliedRotation);
}

// Z
void ULSystemGeneratorSubsystem::RotatePositiveYaw(AActor& Actor)
{
	FRotator AppliedRotation(0,Angle,0);
	//FRotator AppliedRotation(0,0,Angle);
	//Actor.AddActorWorldRotation(AppliedRotation);
	Actor.AddActorLocalRotation(AppliedRotation);

}

void ULSystemGeneratorSubsystem::RotateNegativeYaw(AActor& Actor)
{
	FRotator AppliedRotation(0,-Angle,0);
	//FRotator AppliedRotation(0,0,-Angle);
	//Actor.AddActorWorldRotation(AppliedRotation);
	Actor.AddActorLocalRotation(AppliedRotation);
}

// Y
void ULSystemGeneratorSubsystem::RotatePositivePitch(AActor& Actor)
{
	FRotator AppliedRotation(Angle,0,0);
	//FRotator AppliedRotation(-Angle,0,0);
	//Actor.AddActorWorldRotation(AppliedRotation);
	Actor.AddActorLocalRotation(AppliedRotation);

}

void ULSystemGeneratorSubsystem::RotateNegativePitch(AActor& Actor)
{
	FRotator AppliedRotation(-Angle,0,0);
	//FRotator AppliedRotation(Angle,0,0);
	//Actor.AddActorWorldRotation(AppliedRotation);
	Actor.AddActorLocalRotation(AppliedRotation);

}

/*Remove all lines. Used to re-draw trees.*/
void ULSystemGeneratorSubsystem::FlushLines()
{
	GetWorld()->Exec(GetWorld(), *ClearLinesCommand);
}

/*Clear all functions from map.*/
void ULSystemGeneratorSubsystem::ResetFunctions()
{
	TurtleFunctions.Empty();
}

/*Clear all rules from map.*/
void ULSystemGeneratorSubsystem::ResetRules()
{
	Rules.Empty();
}

/*Single generation, carries on from previous iteration.*/
void ULSystemGeneratorSubsystem::Generate()
{
	FString NewString = "";
	TArray<TCHAR> CharacterArray = CurrentString.GetCharArray();
	//UE_LOG(LogTemp, Warning, TEXT("Current string: %s"), *CurrentString);

	// If Character is variable in a rule, replace it with output
	for(int32 i = 0; i < CharacterArray.Num(); i++)
	{
		TCHAR CurrentCharacter = CharacterArray[i];
		if(Rules.Contains(CurrentCharacter))
		{
			NewString += Rules[CurrentCharacter];
			continue;
		}

		// Otherwise add it back to string
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

	OnGenerateDelegate.Broadcast();
}

/*Allows passing l-system values through blueprints.*/
void ULSystemGeneratorSubsystem::SetDefaults(FString UserAxiom, float UserAngle, float UserLineLength, float UserLineThickness)
{
	Axiom = UserAxiom;
	CurrentString = Axiom;
	Angle = UserAngle;
	LineLength = UserLineLength;
	LineThickness = UserLineThickness;
}

/*Add a rule from blueprints.*/
bool ULSystemGeneratorSubsystem::AddRule(FString Input, FString Output)
{
	if(Input.IsEmpty())
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
	}
	
	TArray<TCHAR> CharArray = Input.GetCharArray();

	Rules.Add(CharArray[0], Output);
	UE_LOG(LogTemp, Warning, TEXT("Added rule: %s -> %s"), *Input, *Output);
	return true;
}

/*Add function from blueprints.*/
bool ULSystemGeneratorSubsystem::AddFunction(FString Input, FunctionType TurtleFunction)
{
	if(Input.IsEmpty() || 1 < Input.Len())
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid Function Insertion."));
		return false;
	}
	
	TArray<TCHAR> CharArray = Input.GetCharArray();
	TurtleFunctions.Add(CharArray[0], TurtleFunction);
	UE_LOG(LogTemp, Warning, TEXT("Added function: %s"), *Input);
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
		case FunctionType::RotatePositiveRoll:
			RotatePositiveRoll(Actor);
			break;
		case FunctionType::RotateNegativeRoll:
			RotateNegativeRoll(Actor);
			break;
		case FunctionType::Cache:
			TransformStack.Push(Actor.GetTransform());
			break;
		case FunctionType::ReturnCache:
			Actor.SetActorTransform(TransformStack.Pop());
			break;
		case FunctionType::RotatePositiveYaw:
			RotatePositiveYaw(Actor);
			break;
		case FunctionType::RotateNegativeYaw:
			RotateNegativeYaw(Actor);
			break;
		case FunctionType::RotatePositivePitch:
			RotatePositivePitch(Actor);
			break;
		case FunctionType::RotateNegativePitch:
			RotateNegativePitch(Actor);
			break;
		}
	}
}
