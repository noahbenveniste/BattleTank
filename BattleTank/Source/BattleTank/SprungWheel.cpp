// Fill out your copyright notice in the Description page of Project Settings.

#include "SprungWheel.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/World.h"

// Sets default values
ASprungWheel::ASprungWheel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set the tick group to be post-physics so tick runs after on hit, allowing us to reset the force mag in tick
	PrimaryActorTick.TickGroup = TG_PostPhysics;

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
	
	// Need this for the wheel to generate on hit events
	this->Wheel->SetNotifyRigidBodyCollision(true);

	// Register the wheel as a dynamic delegate for on hit events. The SprungWheel
	// itself as an actor, so it cannot be a delegate.
	this->Wheel->OnComponentHit.AddDynamic(this, &ASprungWheel::OnHit);

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

	// Sanity check to ensure that we're in the correct physics group
	if (GetWorld()->TickGroup == TG_PostPhysics)
	{
		// Reset the force magnitude for the next frame
		this->TotalForceMagnitudeThisFrame = 0;
	}
}

void ASprungWheel::AddDriveForce(float ForceMagnitude)
{
	// Update the force magnitude. Note that the += operator is significant
	// because we call this function multiple times in a row in the movement
	// component, need to add those results up.
	this->TotalForceMagnitudeThisFrame += ForceMagnitude;
}

void ASprungWheel::OnHit(UPrimitiveComponent * HitComponent, AActor * OtherActor, UPrimitiveComponent * OtherComponent, FVector NormalImpulse, const FHitResult & Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("Wheel force applied"));

	// Only apply forces when the wheels are touching ground
	ApplyForce();
}

void ASprungWheel::ApplyForce()
{
	this->Wheel->AddForce(this->Axle->GetForwardVector() * this->TotalForceMagnitudeThisFrame);
}
