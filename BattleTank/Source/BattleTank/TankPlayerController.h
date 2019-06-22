// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h" // Must be the last include

class ATank; // Forward declaration, makes dependencies explicit without creating a chain of dependencies

/**
 * 
 */
UCLASS()
class BATTLETANK_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = Setup)
	void FoundAimingComponent(UTankAimingComponent* AimCompRef);

public:
	UFUNCTION(BlueprintCallable, Category = Setup)
	ATank* GetControlledTank() const;

	void BeginPlay() override;
	void Tick(float DeltaTime) override;

	/**
	 * Start moving the tank barrel so that a shot would hit
	 * where the crosshair intersects with the world
	 */
	void AimTowardsCrosshair();

private:
	/**
	 * Function that linetraces perpendicular to where the crosshair
	 * is on the screen and checks to see if the linetrace intersects
	 * with anything.
	 * @param HitLocation - An FVector out parameter that is updated
	 *                      with the location of where the ray cast hit
	 * @return whether or not the ray trace intersected with anything
	 */
	bool GetSightRayHitLocation(FVector& OutHitLocation) const;

	/**
	 * Function for calculating the direction that the crosshair is facing
	 * in the world
	 * @param ScreenLocation - a 2D vector containg the pixel coordinates of
	 *						   the crosshair on the player's screen
	 * @param LookDirection - reference to an out parameter that will contain
	 *						  a 3D unit vector that represents the direction
	 *                        in the world that the player is facing, based
	 *                        on the crosshair
	 * @return false if the direction can't be obatined, true otherwise
	 */
	bool GetLookDirection(FVector2D ScreenLocation, FVector& LookDirection) const;

	// Range for player aim (10 km in cm)
	UPROPERTY(EditAnywhere)
	float LineTraceRange = 1000000;

	/**
	 *
	 */
	bool GetLookVectorHitLocation(FVector LookDirection, FVector& OutHitLocation) const;
};
