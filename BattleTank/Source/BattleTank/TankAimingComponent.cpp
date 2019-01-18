// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAimingComponent.h"
#include "TankBarrel.h" // Need to include this here because we need to actually use code from this file in the cpp file
						// Including this in the header could create dependency chains. In general, you only want to #include
						// in a header if you're inheriting from something. We used a forward declaration in the header file
						// so we could talk about UTankBarrel in some of the method declarations.


// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UTankAimingComponent::AimAt(FVector HitLocation, float LaunchSpeed)
{

	if (!Barrel)
	{
		return;
	}

	auto OurTankName = GetOwner()->GetName();
	// UE_LOG(LogTemp, Warning, TEXT("%s aiming at %s from location %s"), *OurTankName, *HitLocation.ToString(), *Barrel->GetComponentLocation().ToString());
	DrawDebugLine(
		GetWorld(),
		HitLocation,
		Barrel->GetComponentLocation(),
		FColor(255, 0, 0),
		false, -1, 0,
		12.333
	);

	FVector LaunchVelocity; // Out parameter, what we want to calculate
	FVector StartLocation = Barrel->GetSocketLocation(FName("Projectile")); // Projectile socket on barrel static mesh

	FCollisionResponseParams ResponseParam;
	TArray < AActor * > ActorsToIgnore;

	// Given a location that we're aiming at and a projectile speed, calculate a launch velocity vector that would land the projectile at that location
	if (UGameplayStatics::SuggestProjectileVelocity(
		this, // The world context is just a reference to this instance of the tank aiming component
		LaunchVelocity,
		StartLocation,
		HitLocation,
		LaunchSpeed, // Passed to us by the tank
		false, // want to use low arc, not high arc projectile path
		0, // Collision radius
		0, // OverrideGravityZ
		ESuggestProjVelocityTraceOption::DoNotTrace // Enum that specifies how we want to trace the path
	)) 
	{
		auto AimDirection = LaunchVelocity.GetSafeNormal(); // Converts the velocity vector into a unit vector so we can set the barrel's orientation
		// UE_LOG(LogTemp, Warning, TEXT("%s is aiming in direction %s"), *OurTankName, *AimDirection.ToString());
		MoveBarrelTowards(AimDirection);
	}

}

void UTankAimingComponent::SetBarrelReference(UTankBarrel * BarrelToSet)
{
	this->Barrel = BarrelToSet;
}

void UTankAimingComponent::MoveBarrelTowards(FVector AimDirection)
{
	// Determine difference between current barrel rotation and AimDirection
	auto BarrelRotator = Barrel->GetForwardVector().Rotation(); // Gets the roll, pitch and yaw of the barrel at this moment
	auto AimAsRotator = AimDirection.Rotation();
	auto DeltaRotator = AimAsRotator - BarrelRotator; // This is the aim as a rotation relative to the current orientation of the tank's barrel

	Barrel->Elevate(5); // TODO: Remove magic number
}

