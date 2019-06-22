// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TankAIController.generated.h"

class ATank; // Forward declaration, makes dependencies explicit without creating a chain of dependencies

/**
 * 
 */
UCLASS()
class BATTLETANK_API ATankAIController : public AAIController
{
	GENERATED_BODY()

private:	
	// Override the BeginPlay function to do specific things when the game starts
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaTime) override;
	
	void AimTowardsPlayer();
	// Get the tank that the player controller is possessing; used for aiming and things like that
	APawn* GetPlayerTank() const;

	UPROPERTY(EditDefaultsOnly, Category = Firing)
	float MinimumEngagementDistance = 3000;  // How close the AI tank can get to the player tank
};
