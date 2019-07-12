// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/NavMovementComponent.h"
#include "HoverTankMovementComponent.generated.h"

/* Forward declaration */
class UThrusterComponent;

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BATTLETANK_API UHoverTankMovementComponent : public UNavMovementComponent
{
	GENERATED_BODY()
		
public:
	UHoverTankMovementComponent();

	UFUNCTION(BlueprintCallable, Category = Setup)
	void Initialize(UThrusterComponent* LeftThrusterToSet, UThrusterComponent* RightThrusterToSet);

	UFUNCTION(BlueprintCallable, Category = Input)
	void IntendMoveForward(float Throw);

	UFUNCTION(BlueprintCallable, Category = Input)
	void IntendTurnRight(float Throw);

protected:

private:
	float LeftSideForceMagnitudeThisFrame = 0;

	float RightSideForceMagnitudeThisFrame = 0;

	// Called from the pathfinding logic by the AI controllers
	virtual void RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed) override;
	
	UThrusterComponent* LeftThruster = nullptr;

	UThrusterComponent* RightThruster = nullptr;
};
