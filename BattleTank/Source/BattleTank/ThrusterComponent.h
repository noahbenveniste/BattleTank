// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "ThrusterComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BATTLETANK_API UThrusterComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UThrusterComponent();

	UPROPERTY(EditDefaultsOnly)
	float MaxThrustForce = 75000;

	UFUNCTION(BlueprintCallable, Category = Input)
	void SetThrust(float Throttle);

	void ApplyForce(float Throttle);

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	float TotalForceMagnitudeThisFrame = 0;

};
