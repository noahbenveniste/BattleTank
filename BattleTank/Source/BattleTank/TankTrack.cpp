// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTrack.h"

void UTankTrack::SetThrottle(float Throttle)
{
	auto Name = GetName();
	UE_LOG(LogTemp, Warning, TEXT("%s throttle: %f"), *Name, Throttle);

	// TODO: Clamp throttle

	// Get the forward vector for the track (direction), the maximum magnitude of
	// the force (TrackMaxDrivingForce) and a percentage of that magnitude based on
	// the player's button input (Throttle)
	auto ForceApplied = this->GetForwardVector() * Throttle * TrackMaxDrivingForce;

	// The location to apply the force: the origin of the tank track static mesh
	auto ForceLocation = this->GetComponentLocation();

	// Need to get the root object for what the tread is a part of, i.e. the tank root,
	// then cast this down to  UPrimitiveComponent so we can use the AddForceAtLocation
	// function.
	auto TankRoot = Cast<UPrimitiveComponent>( this->GetOwner()->GetRootComponent() );

	// Apply the force
	TankRoot->AddForceAtLocation(ForceApplied, ForceLocation);
}


