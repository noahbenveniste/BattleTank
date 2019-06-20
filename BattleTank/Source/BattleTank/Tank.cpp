// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"
#include "Projectile.h"
#include "TankBarrel.h"
#include "TankAimingComponent.h"
#include "TankMovementComponent.h"

// Sets default values
ATank::ATank()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Adds the aiming component to the Tank upon construction; visible in blueprint but it can't be removed from
	// the blueprint, which is what we want
	TankAimingComponent = CreateDefaultSubobject<UTankAimingComponent>(FName("Aiming Component"));

	// In lecture 213 we change up the component architecture and do something different with the movement component
	// Now, unlike the aiming component, the movement component doesn't show up by default as an inherited component
	// and is now blueprint spawnable.
	// TankMovementComponent = CreateDefaultSubobject<UTankMovementComponent>(FName("Movement Component"));

}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();

}

// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

UTankAimingComponent * ATank::GetTankAimingComponent()
{
	return TankAimingComponent;
}

void ATank::SetBarrelReference(UTankBarrel * BarrelToSet)
{
	GetTankAimingComponent()->SetBarrelReference(BarrelToSet);
	this->Barrel = BarrelToSet;
}

void ATank::SetTurretReference(UTankTurret * TurretToSet)
{
	GetTankAimingComponent()->SetTurretReference(TurretToSet);
}

void ATank::AimAt(FVector HitLocation)
{
	this->TankAimingComponent->AimAt(HitLocation, this->LaunchSpeed);
}

void ATank::Fire()
{
	// Get the current time and subtract the time we last fired at, check if the reload time has elapsed
	bool isReloaded = (FPlatformTime::Seconds() - LastFiredTime) > ReloadTimeInSeconds;

	if (Barrel && isReloaded) // Check if barrel reference was set and that we're ready to fire again
	{
		// Spawn projectile at barrel socket location
		// Remember, AProjectile is a class that we created that inherits from actor
		auto Projectile = GetWorld()->SpawnActor<AProjectile>(
			this->ProjectileBlueprint, // The class type we want to spawn, as specified in the Projectile blueprint (or use default specified in header)
			this->Barrel->GetSocketLocation(FName("Projectile")), // Location of where we want to spawn the actor (the socket is on the TankBarrel static mesh)
			this->Barrel->GetSocketRotation(FName("Projectile")) // Rotation for the spawned actor (the socket is on the TankBarrel static mesh)
			);

		// Each projectile has a default sub object ProjectileMotionComponent. Call the
		// LaunchProjectile method from this component on the AProjectile that we just spawned.
		// LaunchSpeed is defined in our header file.
		Projectile->LaunchProjectile(LaunchSpeed);

		LastFiredTime = FPlatformTime::Seconds(); // Set the time for when we last fired
	}

}
