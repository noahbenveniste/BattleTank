// Fill out your copyright notice in the Description page of Project Settings.

#include "HoverTankMovementComponent.h"
#include "Engine/World.h"
#include "ThrusterComponent.h"

UHoverTankMovementComponent::UHoverTankMovementComponent()
{
	// Set the tick group to be post-physics so tick runs after on hit, allowing us to reset the force mag in tick
	PrimaryComponentTick.TickGroup = TG_PostPhysics;
}

void UHoverTankMovementComponent::Initialize(UThrusterComponent * LeftThrusterToSet, UThrusterComponent * RightThrusterToSet)
{
	this->LeftThruster = LeftThrusterToSet;
	this->RightThruster = RightThrusterToSet;

	UE_LOG(LogTemp, Warning, TEXT("%s successful HoverTankMovementComponent initialization"), *(GetOwner()->GetName()));
}

void UHoverTankMovementComponent::IntendMoveForward(float Throw)
{
	if (!(this->LeftThruster && this->RightThruster))
	{
		UE_LOG(LogTemp, Warning, TEXT("One or more thruster component references not set for %s"), *(GetOwner()->GetName()));
		return;
	}
	this->LeftThruster->SetThrust(Throw);
	this->RightThruster->SetThrust(Throw);
}

void UHoverTankMovementComponent::IntendTurnRight(float Throw)
{
	if (!(this->LeftThruster && this->RightThruster))
	{
		UE_LOG(LogTemp, Warning, TEXT("One or more thruster component references not set for %s"), *(GetOwner()->GetName()));
		return;
	}
	this->LeftThruster->SetThrust(Throw);
	this->RightThruster->SetThrust(-Throw);
}

void UHoverTankMovementComponent::RequestDirectMove(const FVector & MoveVelocity, bool bForceMaxSpeed)
{
	// Don't need to call Super() because we're replacing that functionality

	// Get the direction the tank is currently facing (as a unit vector)
	auto TankCurrentDirection = GetOwner()->GetActorForwardVector().GetSafeNormal();

	// The direction the tank intends to go (as a unit vector)
	auto TankIntendedDirection = MoveVelocity.GetSafeNormal();

	// The cosine of the angle between these two vectors gives us a value that we can pass to IntendMoveForward (see lecture 217 on dot products for explanation)
	// Because these are unit vectors, the dot product of them gives us the cosine of the angle
	auto ForwardThrow = FVector::DotProduct(TankCurrentDirection, TankIntendedDirection);
	IntendMoveForward(ForwardThrow);

	// We can do something similar with the sine of the angle between the two vectors and IntendMoveRight by using the cross product
	auto Resultant = FVector::CrossProduct(TankCurrentDirection, TankIntendedDirection);

	// The sine of the angle is just equal to the z component of the Resultant vector. This works because the two vectors we original crossed were unit vectors.
	auto RightThrow = Resultant.Z;
	IntendTurnRight(RightThrow);
}