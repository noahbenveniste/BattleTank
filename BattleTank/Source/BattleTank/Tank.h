// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"
#include "Tank.generated.h"

UCLASS()
class BATTLETANK_API ATank : public APawn
{
	GENERATED_BODY()
private:
	// Sets default values for this pawn's properties
	ATank();

	virtual void BeginPlay() override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser) override;

	UPROPERTY(EditDefaultsOnly, Category = Setup)
	int32 StartingHealth = 100;

	UPROPERTY(VisibleAnywhere, Category = Health)
	int32 CurrentHealth = StartingHealth;
};
