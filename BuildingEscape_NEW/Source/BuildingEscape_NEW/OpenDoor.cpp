// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape_NEW.h"
#include "OpenDoor.h"
#include "Grabber.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner();
	if (!PressurePlate) 
	{
		UE_LOG(LogTemp, Error, TEXT("%missing Pressure Plate"), *GetOwner()->GetName());
	}
}

void UOpenDoor::CloseDoor()
{
	//Set the door rotation	
	Owner->SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll the trigger volume every frame
	if (GetTotalMassOfActorsOnPlate() > TriggerMass) 
	{
		OnOpenRequest.Broadcast();
	}

	else 
	{
	    CloseDoor();
	}	
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.f;
	
	//Find all the other overlapping actors
	TArray<AActor*> OverlappingActors;
	if (!PressurePlate) { return TotalMass; } //we need to return a float to get out of the function
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	//Iterate through them adding their masses
	for (const auto *Actor : OverlappingActors) 
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("%s on pressure plate"), *Actor->GetName());
	}

	return TotalMass;
}


