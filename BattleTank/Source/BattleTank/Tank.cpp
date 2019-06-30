// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"

// Sets default values
ATank::ATank()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Adds the aiming component to the Tank upon construction; visible in blueprint but it can't be removed from
	// the blueprint, which is what we want
	// No longer doing default sub object with either component
	// TankAimingComponent = CreateDefaultSubobject<UTankAimingComponent>(FName("Aiming Component"));

	// In lecture 213 we change up the component architecture and do something different with the movement component
	// Now, unlike the aiming component, the movement component doesn't show up by default as an inherited component
	// and is now blueprint spawnable.
	// TankMovementComponent = CreateDefaultSubobject<UTankMovementComponent>(FName("Movement Component"));
}

void ATank::BeginPlay()
{
	// Need to call this to get the Tank_BP BeginPlay event to run ONLY because we've
	// overridden it. If we didn't override then we could just not include this. I'm 
	// leaving it in just as a note for myself.
	Super::BeginPlay();
}

float ATank::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	// Clamp DamageAmount between 0 and the Tank's health i.e. if it is greater than the Tank's health, it
	// won't cause health to go negative.
	int32 DamagePoints = FPlatformMath::RoundToInt(DamageAmount);
	auto DamageToApply = FMath::Clamp(DamagePoints, 0, CurrentHealth);

	// Apply the damage
	CurrentHealth -= DamageToApply;

	return DamageToApply;
}

float ATank::GetHealthPercent() const
{
	return (float)CurrentHealth / (float)StartingHealth;
}

