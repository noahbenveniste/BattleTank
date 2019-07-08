// Fill out your copyright notice in the Description page of Project Settings.

#include "SprungWheel.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"


// Sets default values
ASprungWheel::ASprungWheel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create the spring component i.e. the physics constraint, make it the scene root
	this->Spring = CreateDefaultSubobject<UPhysicsConstraintComponent>(FName("Spring"));
	SetRootComponent(this->Spring);

	// Create the wheel component
	this->Wheel = CreateDefaultSubobject<UStaticMeshComponent>(FName("Wheel"));

	// Attach the mass and wheel to the spring
    // Better than using AttachToComponent for setup, can only be used in constructors
	this->Wheel->SetupAttachment(this->Spring);
}

// Called when the game starts or when spawned
void ASprungWheel::BeginPlay()
{
	Super::BeginPlay();
	
	SetupConstraint();
}

void ASprungWheel::SetupConstraint()
{
	// Pointer protection
	if (!GetAttachParentActor()) { return; }

	// Attach the spring physics constraint to the tank
	UPrimitiveComponent* BodyRoot = Cast<UPrimitiveComponent>(GetAttachParentActor()->GetRootComponent());

	// More pointer protection
	if (!BodyRoot) { return; }

	// Set the constraint
	this->Spring->SetConstrainedComponents(BodyRoot, NAME_None, Cast<UPrimitiveComponent>(Wheel), NAME_None);
}

// Called every frame
void ASprungWheel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

