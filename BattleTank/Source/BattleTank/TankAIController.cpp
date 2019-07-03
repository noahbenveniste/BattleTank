// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "TankAimingComponent.h"
#include "Tank.h" // So we can implement OnDeath
// Implicit dependency on movement component via pathfinding logic

void ATankAIController::BeginPlay()
{
	Super::BeginPlay(); // Ensure that the BeginPlay that we are overriding is run first

	// Some code to test that we are correctly possessing a tank
	auto ControlledTank = GetPawn();
	if (!ControlledTank) // If the thing this points to is null, a tank isn't being possessed
	{
		UE_LOG(LogTemp, Warning, TEXT("TankAIController not possessing a tank"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("TankAIController possessing: %s"), *ControlledTank->GetName());
	}

	// Try to get the player tank
	auto PlayerTank = GetPlayerTank();
	
	// Check for nullptr
	if (!ensure(PlayerTank))
	{
		UE_LOG(LogTemp, Warning, TEXT("No player tank found"));
	}
	else
	{
		// Report that we have a reference to the player tank
		UE_LOG(LogTemp, Warning, TEXT("Player tank found: %s"), *PlayerTank->GetName());
	}
	
}

void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime); // Call superclass's Tick function first
	
	// Pointer protection for this tank and the player tank
	if (!(GetPlayerTank() && GetPawn())) { return; }
	
	// Move towards player
	MoveToActor(GetPlayerTank(), AcceptanceRadius);

	// Aim towards player
	AimTowardsPlayer();

	// Get the aiming component
	auto AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
	// Pointer protection
	if (!ensure(AimingComponent)) { return; }
	// Check if aiming or locked
	if (AimingComponent->GetCurrentFiringState() == EFiringState::LOCKED)
	{
		AimingComponent->Fire();
	}
}

void ATankAIController::SetPawn(APawn * InPawn)
{
	Super::SetPawn(InPawn);
	if (InPawn)
	{
		auto PossessedTank = Cast<ATank>(InPawn);
		if (!PossessedTank) { return; }

		// Subscribe our local method to the tank's death event
		PossessedTank->OnDeath.AddUniqueDynamic(this, &ATankAIController::OnTankDeath);
	}
}

void ATankAIController::OnTankDeath()
{
	if (!GetPawn()) { return; }
	GetPawn()->DetachFromControllerPendingDestroy();
}

void ATankAIController::AimTowardsPlayer()
{
	// Get the aiming component
	auto AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
	// Pointer protection
	if (!ensure(AimingComponent)) { return; }
	AimingComponent->AimAt(GetPlayerTank()->GetTargetLocation());
}

APawn* ATankAIController::GetPlayerTank() const
{
	return GetWorld()->GetFirstPlayerController()->GetPawn();
}


