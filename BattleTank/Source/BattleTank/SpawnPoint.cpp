// Fill out your copyright notice in the Description page of Project Settings.

#include "SpawnPoint.h"
#include "Engine/World.h"
#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
USpawnPoint::USpawnPoint()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USpawnPoint::BeginPlay()
{
	Super::BeginPlay();

	// Spawn the new actor
	auto NewActor = GetWorld()->SpawnActorDeferred<AActor>(this->SpawnClass, GetComponentTransform());

	// Pointer protection
	if (!NewActor) { return; }

	// Attach the actor to this component
	NewActor->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform); // Use KeepWorldTransform because we've already set the spawn location in SpawnActorDeferred

	// Finish spawning
	UGameplayStatics::FinishSpawningActor(NewActor, GetComponentTransform());

	// Store the actor that we just spawned in the member variable
	this->SpawnedActor = NewActor;
}


// Called every frame
void USpawnPoint::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

AActor * USpawnPoint::GetSpawnedActor() const
{
	return this->SpawnedActor;
}

