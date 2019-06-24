// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Components/ActorComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TankAimingComponent.generated.h"

// Enum for aiming state

UENUM()
enum class EFiringState : uint8
{
	LOCKED,
	AIMING,
	RELOADING
};

class UTankBarrel; // Forward declaration, makes dependencies explicit without creating a chain of dependencies
class UTankTurret;
class AProjectile;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BATTLETANK_API UTankAimingComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	// Tank's current firing status; defaults to reloading
	UPROPERTY(BlueprintReadOnly, Category = State)
	EFiringState CurrentFiringState = EFiringState::RELOADING;

public:
	UFUNCTION(BlueprintCallable, Category = Firing)
	void Fire();

	// Sets default values for this component's properties
	UTankAimingComponent();

	UFUNCTION(BlueprintCallable, Category = Setup)
	void Initialise(UTankBarrel* BarrelToSet, UTankTurret* TurretToSet);

	void AimAt(FVector HitLocation);
	
private:
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

	// The last time the gun was fired; used for limiting the fire rate of the tank's gun
	double LastFiredTime = 0;

	UTankBarrel* Barrel = nullptr;

	UTankTurret* Turret = nullptr;

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction);

	bool IsBarrelMoving();

	FVector AimDirection;

	void MoveBarrelTowards(FVector AimDirection);

	void RotateTurretTowards(FVector AimDirection);
};
