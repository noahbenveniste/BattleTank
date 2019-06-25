// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile.h"


// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create default sub object UStaticMeshComponent called CollisionMesh
	this->CollisionMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Collision Mesh"));

	// Set the collision mesh to be the root component
	SetRootComponent(this->CollisionMesh);

	// Make "simulation generates hit events" the default
	this->CollisionMesh->SetNotifyRigidBodyCollision(true);

	// Want the visibility of the collision mesh to be off by default
	this->CollisionMesh->SetVisibility(false);

	// Create default sub object UParticleSystemComponent called LaunchBlast
	this->LaunchBlast = CreateDefaultSubobject<UParticleSystemComponent>(FName("Launch Blast"));

	// Attach the launch blast to the root component
	this->LaunchBlast->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	// Adds a default sub object upon construction so that it can't be removed (similar to Tank and TankAimingComponent)
	this->ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(FName("Projectile Movement Component"));
	this->ProjectileMovementComponent->bAutoActivate = false;
}

void AProjectile::LaunchProjectile(float Speed)
{
	ProjectileMovementComponent->SetVelocityInLocalSpace(FVector::ForwardVector * Speed);
	ProjectileMovementComponent->Activate();
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

