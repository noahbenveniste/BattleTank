// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TankAimingComponent.h"
#include "GameFramework/Pawn.h"
#include "Tank.generated.h"

UCLASS()
class BATTLETANK_API ATank : public APawn
{
	GENERATED_BODY()

protected:
	UTankAimingComponent* TankAimingComponent = nullptr;

public:
	UTankAimingComponent* GetTankAimingComponent();

	/**
	 * Function that is called from the Tank blueprint's event graph
	 * to set the reference to the barrel static mesh component in the blueprint
	 * so we can manipulate it in C++. Need this helper function in
	 * addition to the one in TankAimingComponent because we only have
	 * a blueprint even graph for the Tank where we can call this function,
	 * not one for the aiming component. Normally these types of message
	 * chains are bad practice but in this case it helps promote encapsulation.
	 */
	UFUNCTION(BlueprintCallable, Category = Setup)
	void SetBarrelReference(UStaticMeshComponent *BarrelToSet);

private:
	// Sets default values for this pawn's properties
	ATank();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	
};
