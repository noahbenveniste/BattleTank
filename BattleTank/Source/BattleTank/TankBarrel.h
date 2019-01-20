// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "TankBarrel.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), hidecategories = "collision")
class BATTLETANK_API UTankBarrel : public UStaticMeshComponent
{
	GENERATED_BODY()
	
public:
	// Elevates the barrel given a relative speed multiplier i.e.
	// +1 would be max upward elevation change and -1 would be max
	// downward elevation change
	void Elevate(float RelativeSpeed);
	
private:
	// How fast the barrel's elevation can change
	UPROPERTY(EditAnywhere, Category = Setup)
	float MaxDegreesPerSecond = 10;

	// The max pitch the barrel can go to
	UPROPERTY(EditAnywhere, Category = Setup)
	float MaxElevationDegrees = 40;

	// The min pitch the barrel can go to (any lower and the barrel clips into the tank body)
	UPROPERTY(EditAnywhere, Category = Setup)
	float MinElevationDegrees = 0;
};
