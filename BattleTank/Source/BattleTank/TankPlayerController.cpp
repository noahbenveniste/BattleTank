// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"
#include "BattleTank.h"


void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay(); // Ensure that the BeginPlay that we are overriding is run first
	
	// Some code to test that we are correctly possessing a tank
	auto ControlledTank = GetControlledTank();
	if (!ControlledTank)
	{
		UE_LOG(LogTemp, Warning, TEXT("TankPlayerController not possessing a tank"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("TankPlayerController possessing: %s"), *ControlledTank->GetName());
	}
	
}

void ATankPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime); // Call superclass's Tick function first
	AimTowardsCrosshair();
}

ATank* ATankPlayerController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
}

void ATankPlayerController::AimTowardsCrosshair()
{
	if (!GetControlledTank()) // If we aren't possessing a tank, just return
	{
		return;
	}

	FVector HitLocation; // Out parameter

	// Get world location of linetrace through crosshair
	if (GetSightRayHitLocation(HitLocation)) // If linetrace hits the landscape
	{
		// Tell controlled tank to aim at this point
		GetControlledTank()->AimAt(HitLocation);
	}

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
	if (GetLookDirection(ScreenLocation, CameraLookDirection))
	{
		// Ray cast along that look direction and see what we hit (up to a max range),
		// get the location of that hit (if there is one),
		// return boolean result: true if we hit something, false otherwise
		return GetLookVectorHitLocation(CameraLookDirection, OutHitLocation);
	}
	else
	{
		OutHitLocation = FVector(0); // If we aren't aiming at anything
		return false;
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
	return false; // Line trace didn't succeed
}
