// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "TankAimingComponent.h"
// Implicit dependency on movement component via pathfinding logic

void ATankAIController::BeginPlay()
{
	Super::BeginPlay(); // Ensure that the BeginPlay that we are overriding is run first

	// Some code to test that we are correctly possessing a tank
	auto ControlledTank = GetPawn();
	if (!ensure(ControlledTank)) // If the thing this points to is null, a tank isn't being possessed
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
	if (!ensure(GetPlayerTank() && GetPawn())) { return; }
	
	// Move towards player
	MoveToActor(GetPlayerTank(), MinimumEngagementDistance);

	// Aim towards player
	AimTowardsPlayer();

	// TODO: Refactor this into a FireAtPlayer() method
	// Get the aiming component
	auto AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
	// Pointer protection
	if (!ensure(AimingComponent)) { return; }
	// Fire
	AimingComponent->Fire();
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


