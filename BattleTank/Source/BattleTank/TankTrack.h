// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "TankTrack.generated.h"

/**
 * TankTrack is used to set maximum driving force and to apply forces to the tank.
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BATTLETANK_API UTankTrack : public UStaticMeshComponent
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = Input)
	void SetThrottle(float Throttle);
	
	// Max force per track in newtons
	// A tank is roughly 45000 kilos, the Abrams can go 0-20 mph
	// in 7 seconds which works out to an acceleration of roughly 1.3 m/s/s,
	// so the force required is 45000 * 1.3 = 58500, which based on
	// our precision I'll round down to 58000
	// Also this assumes no friction.
	UPROPERTY(EditDefaultsOnly)
	float TrackMaxDrivingForce = 25000;
};
