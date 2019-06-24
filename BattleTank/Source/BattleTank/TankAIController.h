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

protected:
	// Note: must be in protected for blueprint to access b/c blueprint is a child class of the C++ class
	UPROPERTY(EditDefaultsOnly, Category = Setup)
	float AcceptanceRadius = 8000;  // How close the AI tank can get to the player tank

	// Note about UPROPERTY(): The value we define in C++ defines what will appear in the blueprint text box.
	// Whatever we actually put in the blueprint text box will override the default C++ value.

private:	
	// Override the BeginPlay function to do specific things when the game starts
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaTime) override;
	
	void AimTowardsPlayer();
	// Get the tank that the player controller is possessing; used for aiming and things like that
	APawn* GetPlayerTank() const;
};
