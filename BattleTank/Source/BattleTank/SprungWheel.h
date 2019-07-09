// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SprungWheel.generated.h"

/* Forward declaration of UPhysicsConstraintComponent */
class UPhysicsConstraintComponent;

/* Forward declaration of USphereComponent */
class USphereComponent;

UCLASS()
class BATTLETANK_API ASprungWheel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASprungWheel();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void AddDriveForce(float ForceMagnitude);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Spring constraint
	UPROPERTY(VisibleAnywhere, Category = Components)
	UPhysicsConstraintComponent* Spring = nullptr;

	// Axle. Needs to be USphereComponent because we need
	// a mesh to simulate physics, but the sphere component
	// doesn't actually collide with anything
	UPROPERTY(VisibleAnywhere, Category = Components)
	USphereComponent* Axle = nullptr;

	// Wheel
	UPROPERTY(VisibleAnywhere, Category = Components)
	USphereComponent* Wheel = nullptr;

	// Wheel-axle constraint
	UPROPERTY(VisibleAnywhere, Category = Components)
	UPhysicsConstraintComponent* WheelAxleConstraint = nullptr;

private:
	void SetupConstraint();
	
};
