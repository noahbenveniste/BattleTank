// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SprungWheel.generated.h"

/**
 * Forward declaration of UPhysicsConstraintComponent
 */
class UPhysicsConstraintComponent;

UCLASS()
class BATTLETANK_API ASprungWheel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASprungWheel();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Physics constraint
	UPROPERTY(VisibleAnywhere, Category = Components)
	UPhysicsConstraintComponent* Spring = nullptr;

	// Bottom mesh, i.e. the wheel
	UPROPERTY(VisibleAnywhere, Category = Components)
	UStaticMeshComponent* Wheel = nullptr;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void SetupConstraint();
	
};
