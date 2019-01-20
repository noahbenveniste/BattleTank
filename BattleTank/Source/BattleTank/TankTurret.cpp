// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTurret.h"

void UTankTurret::Rotate(float RelativeSpeed)
{
	// Clamp relative speed between -1 and +1
	float ClampedRelativeSpeed = FMath::Clamp<float>(RelativeSpeed, -1, 1);

	// Rotate the turret the correct amount for this tick
	auto RotationChange = ClampedRelativeSpeed * MaxDegreesPerSecond * GetWorld()->DeltaTimeSeconds;
	float NewRotation = RelativeRotation.Yaw + RotationChange; // RelativeRotation is a property of this tank barrel as a scene component

	SetRelativeRotation(FRotator(
		0, // Pitch
		NewRotation, // Yaw
		0  // Roll
	));
}


