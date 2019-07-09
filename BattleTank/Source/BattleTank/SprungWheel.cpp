// Fill out your copyright notice in the Description page of Project Settings.

#include "SprungWheel.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
ASprungWheel::ASprungWheel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create the spring component i.e. the physics constraint, make it the scene root
	this->Spring = CreateDefaultSubobject<UPhysicsConstraintComponent>(FName("Spring"));
	SetRootComponent(this->Spring);

	// Create the axle component
	this->Axle = CreateDefaultSubobject<USphereComponent>(FName("Axle"));

	// Attach the axle to the spring
	// Better than using AttachToComponent for setup, can only be used in constructors
	this->Axle->SetupAttachment(this->Spring);

	// Create the wheel component
	this->Wheel = CreateDefaultSubobject<USphereComponent>(FName("Wheel"));

	// Attach the wheel to the axle
	this->Wheel->SetupAttachment(this->Axle);

	// Create the wheel/axle physics constraint
	this->WheelAxleConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(FName("WheelAxleConstraint"));

	// Attach the wheel/axle constraint and the axle
	this->WheelAxleConstraint->SetupAttachment(this->Axle);
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

	// Set up spring constraint
	this->Spring->SetConstrainedComponents(BodyRoot, NAME_None, Cast<UPrimitiveComponent>(this->Wheel), NAME_None);

	// Set up axle constraint
	this->WheelAxleConstraint->SetConstrainedComponents(Cast<UPrimitiveComponent>(this->Axle), NAME_None, Cast<UPrimitiveComponent>(this->Wheel), NAME_None);
}

// Called every frame
void ASprungWheel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASprungWheel::AddDriveForce(float ForceMagnitude)
{
	this->Wheel->AddForce(this->Axle->GetForwardVector() * ForceMagnitude);
}

