// Fill out your copyright notice in the Description page of Project Settings.

#include "TankMovementComponent.h"
#include "TankTrack.h"

void UTankMovementComponent::Initialise(UTankTrack* LeftTrackToSet, UTankTrack* RightTrackToSet)
{
	LeftTrack = LeftTrackToSet;
	RightTrack = RightTrackToSet;
}

void UTankMovementComponent::IntendMoveForward(float Throw)
{
	if (!ensure(LeftTrack && RightTrack))
	{
		UE_LOG(LogTemp, Error, TEXT("Error: One or more of the TankTrack pointers is null"));
		return;
	}
	LeftTrack->SetThrottle(Throw);
	RightTrack->SetThrottle(Throw);
}

void UTankMovementComponent::IntendTurnRight(float Throw)
{
	if (!ensure(LeftTrack && RightTrack))
	{
		UE_LOG(LogTemp, Error, TEXT("Error: One or more of the TankTrack pointers is null"));
		return;
	}
	LeftTrack->SetThrottle(Throw);
	RightTrack->SetThrottle(-Throw);
}

void UTankMovementComponent::RequestDirectMove(const FVector & MoveVelocity, bool bForceMaxSpeed)
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




