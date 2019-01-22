// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Tank.generated.h"

// Forward declarations of class types that we need to reference in this header.
// Never #include anything in a header file unless you are inheriting from it.
// This prevents dependencies chains and decouples our architecture. If we need
// to use any of these classes in the implementation, we'll #include them in the
// actuall cpp file.
class UTankAimingComponent;
class UTankBarrel;
class UTankTurret;
class AProjectile;

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
	void SetBarrelReference(UTankBarrel *BarrelToSet);

	/**
	 * Another function similar to the above one, but specifically for the tank's turret
	 */
	UFUNCTION(BlueprintCallable, Category = Setup)
	void SetTurretReference(UTankTurret *TurretToSet);

	void AimAt(FVector HitLocation);

	UFUNCTION(BlueprintCallable)
	void Fire();

private:
	// Sets default values for this pawn's properties
	ATank();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	// Initiailize gun projectile speed; can be edited in blueprint
	// EditDefaultsOnly means we can't edit these values on a tank by tank
	// basis, just the default value for all tank instances
	UPROPERTY(EditDefaultsOnly, Category = Firing)
	float LaunchSpeed = 100000; // TODO: find reasonable init value

	UPROPERTY(EditDefaultsOnly, Category = Firing)
	float ReloadTimeInSeconds = 3; // TODO: find reasonable init value

	// Gives us the ability to set a projectile type for each tank instance.
	// We do this instead of UClass* so that a designer can only choose a
	// class of type AProjectile that we've designed.
	UPROPERTY(EditAnywhere, Category = Setup)
	TSubclassOf<AProjectile> ProjectileBlueprint;

	// Local barrel reference for spawning projectiles
	UTankBarrel* Barrel = nullptr;
	
	// The last time the gun was fired; used for limiting the fire rate of the tank's gun
	double LastFiredTime = 0;
};
