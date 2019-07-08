// Fill out your copyright notice in the Description page of Project Settings.

#include "SprungWheel.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"


// Sets default values
ASprungWheel::ASprungWheel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create the mass component, set it as the scene root
	this->Mass = CreateDefaultSubobject<UStaticMeshComponent>(FName("Mass"));
	SetRootComponent(this->Mass);

	// Create the spring component i.e. the physics constraint
	this->Spring = CreateDefaultSubobject<UPhysicsConstraintComponent>(FName("Spring"));

	// Create the wheel component
	this->Wheel = CreateDefaultSubobject<UStaticMeshComponent>(FName("Wheel"));

	// Attach the spring and wheel to the mass
	this->Spring->SetupAttachment(this->Mass); // Better than using AttachToComponent for setup, can only be used in constructors
	this->Wheel->SetupAttachment(this->Mass);
}

// Called when the game starts or when spawned
void ASprungWheel::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASprungWheel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

