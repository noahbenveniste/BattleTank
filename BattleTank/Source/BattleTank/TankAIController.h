// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tank.h"
#include "AIController.h"
#include "TankAIController.generated.h"

/**
 * 
 */
UCLASS()
class BATTLETANK_API ATankAIController : public AAIController
{
	GENERATED_BODY()

private:
	// Get the tank that the AI is currently possessing
	ATank* GetControlledTank() const;
	
	// Override the BeginPlay function to do specific things when the game starts
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaTime) override;
	
	void AimTowardsPlayer();
	// Get the tank that the player controller is possessing; used for aiming and things like that
	ATank* GetPlayerTank() const;
};
