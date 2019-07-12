// Fill out your copyright notice in the Description page of Project Settings.

#include "ThrusterComponent.h"
#include "Engine/World.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UThrusterComponent::UThrusterComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// Set the tick group to post physics. This way, we can reset the forces that are being applied to the thruster in tick
	// PrimaryComponentTick.TickGroup == TG_PostPhysics;
}

// Called when the game starts
void UThrusterComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UThrusterComponent::SetThrust(float Throttle)
{
	ApplyForce(FMath::Clamp<float>(Throttle, -1, 1));
}

void UThrusterComponent::ApplyForce(float Throttle)
{
	// Divide by two because we have two thrusters
	auto ForceMagApplied = (Throttle * MaxThrustForce) / 2;

	// Get the owner
	AActor* HoverTank = this->GetOwner();
	if (!HoverTank) { return; }

	// Get the owner's root component
	USceneComponent* RootTemp = HoverTank->GetRootComponent();
	if (!RootTemp) { return; }

	// Cast to static mesh component
	UStaticMeshComponent* HoverTankBody = Cast<UStaticMeshComponent>(RootTemp);
	if (!HoverTankBody) { return; }

	// Get the forward vector of the tank to calculate the direction of the force
	auto ForceApplied = ForceMagApplied * HoverTankBody->GetForwardVector();

	// The location to apply the force is the location of the thruster component
	auto ForceLocation = this->GetComponentLocation();

	// Draw a debug line showing the force vector
	DrawDebugLine(GetWorld(), ForceLocation, ForceLocation - ForceApplied, FColor(255, 0, 0), false, -1, 0, 12.333);

	// Apply the force to the tank body at the location of the thruster
	HoverTankBody->AddForceAtLocation(ForceApplied, ForceLocation, NAME_None);
}

// Called every frame
void UThrusterComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	/*
	// Sanity check to ensure that we're in the correct physics group
	if (GetWorld()->TickGroup == TG_PostPhysics)
	{
		// Reset the force magnitude for the next frame
		this->TotalForceMagnitudeThisFrame = 0;
	}
	*/
}

