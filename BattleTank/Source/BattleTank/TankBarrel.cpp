// Fill out your copyright notice in the Description page of Project Settings.

#include "TankBarrel.h"

void UTankBarrel::Elevate(float RelativeSpeed)
{
	// Clamp relative speed between -1 and +1
	float ClampedRelativeSpeed = FMath::Clamp<float>(RelativeSpeed, -1, 1);

	// Move the barrel the correct amount this frame given a max elevation
	// speed and delta time

	// Multiplying by DeltaTimeSeconds makes this value degrees instead of degrees/second; makes things framerate independent
	auto ElevationChange = ClampedRelativeSpeed * MaxDegreesPerSecond * GetWorld()->DeltaTimeSeconds;
	float RawNewElevation = RelativeRotation.Pitch + ElevationChange; // RelativeRotation is a property of this tank barrel as a scene component
	
	auto ClampedNewElevation = FMath::Clamp<float>(RawNewElevation, MinElevationDegrees, MaxElevationDegrees);

	// This function is for scene components
	SetRelativeRotation(FRotator(
		ClampedNewElevation, // Pitch, elevates the tank barrel, clamped between our min and max pitch
		0, // Yaw
		0  // Roll
	));
}

