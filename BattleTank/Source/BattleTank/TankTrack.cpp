// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTrack.h"

UTankTrack::UTankTrack()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// We need this to be turned on so the OnHit event works
	SetNotifyRigidBodyCollision(true);
}

void UTankTrack::BeginPlay()
{
	// Always call the super method
	Super::BeginPlay();

	// Need to register the delegate for OnHit
	OnComponentHit.AddDynamic(this, &UTankTrack::OnHit);
}

void UTankTrack::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	// The call to super isn't needed here but it's considered best practice. Can cause issues with
	// blueprints if you don't call the superclasses's method.
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Calculate current slippage speed i.e. check to see if there is any velocity component
	// to the right or left (can just check right, left will be negative).
	// Use two known vectors: the component's velocity and right vector.
	// If these vectors are perpendicular i.e. the forward vector has no sideways components,
	// the cos(90) is 0, so no slippage. If the tank was moving entirely sideways then
	// the angle would be 0, cos(0) = 1, so slippage speed would just be the speed of the
	// tank.
	auto SlippageSpeed = FVector::DotProduct(GetRightVector(), GetComponentVelocity());

	// Determine required acceleration this frame to correct for that slippage. Multiply
	// by the negative of the right unit vector to get the direction the acceleration should
	// be applied in.
	auto CorrectingAcceleration = -SlippageSpeed / DeltaTime * GetRightVector();

	// Apply sideways force using F = m * a. Need to get the Tank's mass first though.
	// GetOwner gets the Tank_BP. GetRootComponent gets the root Tank scene component.
	// Need to cast to a static mesh component to get the mass.
	auto TankRoot = Cast<UStaticMeshComponent>(GetOwner()->GetRootComponent());
	
	// Divide by 2 because there are two tracks
	auto CorrectingForce = (TankRoot->GetMass() * CorrectingAcceleration) / 2;

	// Add the correcting force to the TankRoot directly
	TankRoot->AddForce(CorrectingForce);
}

void UTankTrack::OnHit(UPrimitiveComponent * HitComponent, AActor * OtherActor, UPrimitiveComponent * OtherComponent, FVector NormalImpulse, const FHitResult & Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("Track hitting ground"));
}

void UTankTrack::SetThrottle(float Throttle)
{
	auto Name = GetName();
	//UE_LOG(LogTemp, Warning, TEXT("%s throttle: %f"), *Name, Throttle);

	// TODO: Clamp throttle

	// Get the forward vector for the track (direction), the maximum magnitude of
	// the force (TrackMaxDrivingForce) and a percentage of that magnitude based on
	// the player's button input (Throttle)
	auto ForceApplied = this->GetForwardVector() * Throttle * TrackMaxDrivingForce;

	// The location to apply the force: the origin of the tank track static mesh
	auto ForceLocation = this->GetComponentLocation();

	// Need to get the root object for what the tread is a part of, i.e. the tank root,
	// then cast this down to  UPrimitiveComponent so we can use the AddForceAtLocation
	// function.
	auto TankRoot = Cast<UPrimitiveComponent>( this->GetOwner()->GetRootComponent() );

	// Apply the force
	TankRoot->AddForceAtLocation(ForceApplied, ForceLocation);
}

void UTankTrack::OnRegister()
{
	// For whatever reason, just having the tick method in this class isn't working.
	// This function is needed to make the component actually tick.
	Super::OnRegister();
	PrimaryComponentTick.bCanEverTick = true;
}