// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "Engine/World.h"
#include "Tank.h"
#include "TankPlayerController.h"

void ATankAIController::BeginPlay()
{
	Super::BeginPlay(); // Ensure that the BeginPlay that we are overriding is run first

	// Some code to test that we are correctly possessing a tank
	auto ControlledTank = GetControlledTank();
	if (!ControlledTank) // If the thing this points to is null, a tank isn't being possessed
	{
		UE_LOG(LogTemp, Warning, TEXT("TankAIController not possessing a tank"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("TankAIController possessing: %s"), *ControlledTank->GetName());
	}

	// Try to get the player tank
	ATank* PlayerTank = GetPlayerTank();
	
	// Check for nullptr
	if (!PlayerTank)
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
	if (GetPlayerTank())
	{
		// Move towards player
		MoveToActor(GetPlayerTank(), MinimumEngagementDistance);

		// Aim towards player
		AimTowardsPlayer();

		// Fire if ready
		GetControlledTank()->Fire();
	}
}

void ATankAIController::AimTowardsPlayer()
{
	if (!GetControlledTank()) // If we aren't possessing a tank, just return
	{
		return;
	}

	GetControlledTank()->AimAt(GetPlayerTank()->GetTargetLocation());
}

ATank* ATankAIController::GetPlayerTank() const
{
	// Get the world, then get the first player controller, cast it to an ATankPlayerController
	ATankPlayerController* PlayerController = Cast<ATankPlayerController>(GetWorld()->GetFirstPlayerController());

	// Init return variable
	ATank* PlayerTank = nullptr;

	// Check for null pointer
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("No TankPlayerController found"));
	}
	else
	{
		// If a player controller is found, get the controlled tank
		PlayerTank = PlayerController->GetControlledTank();
	}

	return PlayerTank;
}

ATank* ATankAIController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
}


