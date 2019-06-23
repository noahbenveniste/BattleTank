// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAimingComponent.h"
#include "TankBarrel.h" // Need to include this here because we need to actually use code from this file in the cpp file
						// Including this in the header could create dependency chains. In general, you only want to #include
						// in a header if you're inheriting from something. We used a forward declaration in the header file
						// so we could talk about UTankBarrel in some of the method declarations.
#include "TankTurret.h" // Same as the above for TankBarrel
#include "Projectile.h"

// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UTankAimingComponent::BeginPlay()
{
	// Init LastFiredTime to make Tanks not be able to fire as soon as they spawn
	LastFiredTime = FPlatformTime::Seconds();
}

void UTankAimingComponent::Initialise(UTankBarrel * BarrelToSet, UTankTurret * TurretToSet)
{
	Barrel = BarrelToSet;
	Turret = TurretToSet;
}

void UTankAimingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	if ((FPlatformTime::Seconds() - LastFiredTime) > ReloadTimeInSeconds)
	{
		CurrentFiringState = EFiringState::RELOADING;
	}
	// TODO: Handle AIMING and LOCKED states
}

void UTankAimingComponent::AimAt(FVector HitLocation)
{
	if (!ensure(Barrel)) { return; }

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
		LaunchSpeed, // Class constant
		false, // want to use low arc, not high arc projectile path
		0, // Collision radius
		0, // OverrideGravityZ
		ESuggestProjVelocityTraceOption::DoNotTrace // Enum that specifies how we want to trace the path
	)) 
	{
		auto AimDirection = LaunchVelocity.GetSafeNormal(); // Converts the velocity vector into a unit vector so we can set the barrel's orientation
		// UE_LOG(LogTemp, Warning, TEXT("%s is aiming in direction %s"), *OurTankName, *AimDirection.ToString());
		// auto Time = GetWorld()->GetTimeSeconds();
		// UE_LOG(LogTemp, Warning, TEXT("Projectile velocity suggestion found at time %f"), Time);
		MoveBarrelTowards(AimDirection);
		RotateTurretTowards(AimDirection);
	}
	else
	{
		auto Time = GetWorld()->GetTimeSeconds();
		UE_LOG(LogTemp, Warning, TEXT("No suggestion for projectile velocity found at time %f"), Time);
	}

}

void UTankAimingComponent::MoveBarrelTowards(FVector AimDirection)
{
	// Pointer protection
	if (!ensure(Barrel && Turret)) { return; }

	// Determine difference between current barrel rotation and AimDirection
	auto BarrelRotator = Barrel->GetForwardVector().Rotation(); // Gets the roll, pitch and yaw of the barrel at this moment
	auto AimAsRotator = AimDirection.Rotation(); // Convert the aim direction vector into a rotator
	auto DeltaRotator = AimAsRotator - BarrelRotator; // This is the aim as a rotation relative to the current orientation of the tank's barrel

	Barrel->Elevate(DeltaRotator.Pitch);
}

void UTankAimingComponent::RotateTurretTowards(FVector AimDirection)
{
	auto TurretRotator = Turret->GetForwardVector().Rotation();
	auto AimAsRotator = AimDirection.Rotation();
	auto DeltaRotator = AimAsRotator - TurretRotator;

	Turret->Rotate(DeltaRotator.Yaw);
}

void UTankAimingComponent::Fire()
{
	if (CurrentFiringState != EFiringState::RELOADING) // Check if barrel reference was set and that we're ready to fire again
	{
		// Pointer protection
		if (!ensure(Barrel)) { return; }
		if (!ensure(ProjectileBlueprint)) { return; }

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

