// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTrack.h"
#include "Engine/World.h"
#include "SprungWheel.h"
#include "Components/SceneComponent.h"
#include "SpawnPoint.h"

UTankTrack::UTankTrack()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UTankTrack::BeginPlay()
{
	// Always call the super method
	Super::BeginPlay();
}

TArray<ASprungWheel*> UTankTrack::GetWheels() const
{
	// Get the components attached to the tracks. These will be spawn points that spawn the wheels
	TArray<USceneComponent*> ChildComps;
	GetChildrenComponents(true, ChildComps);

	// Init return array
	TArray<ASprungWheel*> Wheels;

	// Iterate over the spawn points we found, get the actor that each spawned
	for (USceneComponent* Child : ChildComps)
	{
		// Cast the child to a SpawnPoint
		USpawnPoint* Spawner = Cast<USpawnPoint>(Child);
		if (!Spawner) { continue; }

		// Get the spawned actor from the spawner
		AActor* SpawnedActor = Spawner->GetSpawnedActor();

		// Cast the spawned actor to SprungWheel
		ASprungWheel* Wheel = Cast<ASprungWheel>(SpawnedActor);
		if (!Wheel) { continue; }

		// Add it to the return array
		Wheels.Add(Wheel);
	}
	
	// Temp for compilation
	return Wheels;
}

void UTankTrack::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	// The call to super isn't needed here but it's considered best practice. Can cause issues with
	// blueprints if you don't call the superclasses's method.
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UTankTrack::SetThrottle(float Throttle)
{
	// Note: clamping the throttle values prevents user from combining WASD input,
	//	     can only input W, A, S or D one at a time. However the driving does
	//       feel a bit smoother. May need to play around with this.

	// float CurrentThrottle = FMath::Clamp<float>(Throttle, -1, 1);
	float CurrentThrottle = Throttle;

	// Drive the track
	DriveTrack(CurrentThrottle);
}

void UTankTrack::DriveTrack(float CurrentThrottle)
{
	// Get the the maximum magnitude of force (TrackMaxDrivingForce) and a percentage of 
	// that magnitude based on the player's button input (Throttle)
	auto ForceApplied = CurrentThrottle * TrackMaxDrivingForce;

	// Get our wheels
	auto Wheels = GetWheels();

	// Divide up the applied force per wheel
	auto ForcePerWheel = ForceApplied / Wheels.Num();

	// Apply the force per wheel on each force
	for (ASprungWheel* Wheel : Wheels)
	{
		Wheel->AddDriveForce(ForcePerWheel);
	}
}

void UTankTrack::OnRegister()
{
	// For whatever reason, just having the tick method in this class isn't working.
	// This function is needed to make the component actually tick.
	Super::OnRegister();
	PrimaryComponentTick.bCanEverTick = true;
}