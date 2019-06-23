// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"
#include "TankAimingComponent.h"
#include "BattleTank.h"


void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay(); // Ensure that the BeginPlay that we are overriding is run first
	
	// Broadcast on our blueprint implementable event when we get the reference to our TankAimingComponent
	auto AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
	if (!ensure(AimingComponent)) { return; }
	FoundAimingComponent(AimingComponent);
}

void ATankPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime); // Call superclass's Tick function first
	AimTowardsCrosshair();
}

void ATankPlayerController::AimTowardsCrosshair()
{
	// If we aren't possessing a tank, just return
	// Note: Need this line (or at least a check for null pointer). Not having it causes Unreal
	//       to hard crash if you have the TankPlayerController BP open and you're not 
	//		 possessing a Tank. Not using an ensure here just makes it so we don't have an error
	//       message spit out every time we compile.
	if (!GetPawn()) { return; }

	// Get the aiming component
	auto AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
	if (!ensure(AimingComponent)) { return; }

	FVector HitLocation; // Out parameter

	// Get the world location coordinates of where a projection from the camera through the crosshair hits

	if (GetSightRayHitLocation(HitLocation)) // If linetrace projection hits a visible object
	{
		// Tell controlled tank to aim at this point
		AimingComponent->AimAt(HitLocation);
	}

	// If what the crosshair is aiming at isn't a visible object or is out of range, don't elevate the tank barrel
}

bool ATankPlayerController::GetSightRayHitLocation(FVector & OutHitLocation) const
{
	// Outparams, viewport size
	int32 ViewportSizeX, ViewportSizeY;
	GetViewportSize(ViewportSizeX, ViewportSizeY);

	// Relative location of the x position of the crosshair
	// i.e. in the middle of the screen horizontally
	UPROPERTY(EditAnywhere) // Expose parameter to blueprint
	float CrosshairXLocation = 0.5;

	// Relative location of the y position of the crosshair
	// i.e. roughtly a third of the way down vertically from
	// the top of the screen
	UPROPERTY(EditAnywhere) // Expose parameter to blueprint
	float CrosshairYLocation = 0.33333;

	// Calculate the actual position of the crosshair in terms of pixels
	// on the player's screen using the relative location of the crosshair
	// and the player's screen size, store the result in a vector
	FVector2D ScreenLocation(CrosshairXLocation * ViewportSizeX, CrosshairYLocation * ViewportSizeY);
	
	// De-project the screen position of the crosshair; tells us the
	// direction in the world that the crosshair is facing
	FVector CameraLookDirection;

	// Deproject the crosshair given its location on the screen (ScreenLocation)
	// and the direction the camera is facing (gives us a vector that effectively
	// goes into the game in the direction the camera is facing through the crosshair).
	if (GetLookDirection(ScreenLocation, CameraLookDirection))
	{
		// Ray cast along that look direction and see what we hit (up to a max range),
		// get the location of that hit (if there is one),
		// return boolean result: true if we hit something, false otherwise
		return GetLookVectorHitLocation(CameraLookDirection, OutHitLocation);
	}
	else
	{
		return false; // Performing the deprojection on the crosshair failed
	}

}

bool ATankPlayerController::GetLookDirection(FVector2D ScreenLocation, FVector & LookDirection) const
{
	// Throwaway outparameter
	FVector CameraWorldLocation;

	// Perform the de-projection of the crosshair
	return DeprojectScreenPositionToWorld(ScreenLocation.X, ScreenLocation.Y, CameraWorldLocation, LookDirection);
	
}

bool ATankPlayerController::GetLookVectorHitLocation(FVector LookDirection, FVector & OutHitLocation) const
{
	FHitResult HitResult;
	auto StartLocation = PlayerCameraManager->GetCameraLocation();
	auto EndLocation = StartLocation + (LookDirection * LineTraceRange);

	if (GetWorld()->LineTraceSingleByChannel(
					HitResult,
					StartLocation,
					EndLocation,
					ECollisionChannel::ECC_Visibility)) // i.e. checks if ray cast intersects with any visible object
	{
		OutHitLocation = HitResult.Location;
		return true;
	}

	// Setting this to be a zero vector causes the turret to reset to neutral position when aiming at something out of range
	// OutHitLocation = FVector(0);
	return false; // Line trace didn't succeed
}
