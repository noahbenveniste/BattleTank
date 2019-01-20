// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"
#include "Projectile.h"
#include "TankBarrel.h"
#include "TankAimingComponent.h"

// Sets default values
ATank::ATank()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Adds the aiming component to the Tank upon construction; visible in blueprint but it can't be removed from
	// the blueprint, which is what we want
	TankAimingComponent = CreateDefaultSubobject<UTankAimingComponent>(FName("Aiming Component"));

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
	UE_LOG(LogTemp, Warning, TEXT("Fire called"));
	
	if (!Barrel) // Check if barrel reference failed to be set
	{
		return;
	}
	else
	{
		// Spawn projectile at barrel socket location
		GetWorld()->SpawnActor<AProjectile>(
			this->ProjectileBlueprint, // The class type we want to spawn, as specified in the Projectile blueprint (or use default specified in header)
			this->Barrel->GetSocketLocation(FName("Projectile")), // Location of where we want to spawn the actor (the socket is on the TankBarrel static mesh)
			this->Barrel->GetSocketRotation(FName("Projectile")) // Rotation for the spawned actor (the socket is on the TankBarrel static mesh)
			);
	}

}
