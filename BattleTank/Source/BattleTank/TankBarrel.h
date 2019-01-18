// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "TankBarrel.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), hidecategories = "collision")
class BATTLETANK_API UTankBarrel : public UStaticMeshComponent
{
	GENERATED_BODY()
	
public:
	void Elevate(float DegreesPerSecond);
	
private:
	// How fast the barrel's elevation can change
	UPROPERTY(EditAnywhere, Category = Setup)
	float MaxDegreesPerSecond = 20;

	// The max pitch the barrel can go to
	UPROPERTY(EditAnywhere, Category = Setup)
	float MaxElevationDegrees = 40;

	// The min pitch the barrel can go to (any lower and the barrel clips into the tank body)
	UPROPERTY(EditAnywhere, Category = Setup)
	float MinElevationDegrees = 0;
};
