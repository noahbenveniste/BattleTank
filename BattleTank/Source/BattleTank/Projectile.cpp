// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile.h"
#include "Runtime/Engine/Classes/PhysicsEngine/RadialForceComponent.h"
#include "Kismet/GameplayStatics.h"


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

	// Create default sub object UParticleSystemComponent called ImpactBlast
	this->ImpactBlast = CreateDefaultSubobject<UParticleSystemComponent>(FName("Impact Blast"));

	// Attach the impact blast to the root component
	this->ImpactBlast->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	this->ImpactBlast->bAutoActivate = false;

	// Create a default subobject for an explosion impulse to move the tanks when they are hit with a projectile
	this->ExplosionForce = CreateDefaultSubobject<URadialForceComponent>(FName("Explosion Force"));
	this->ExplosionForce->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	// Adds a default sub object upon construction so that it can't be removed (similar to Tank and TankAimingComponent)
	this->ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(FName("Projectile Movement Component"));
	this->ProjectileMovementComponent->bAutoActivate = false;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	// Need to register the delegate for OnHit.  The projectile is an actor, not
	// a component. The collision mesh is the component that will be the delegate.
	this->CollisionMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
}

void AProjectile::LaunchProjectile(float Speed)
{
	ProjectileMovementComponent->SetVelocityInLocalSpace(FVector::ForwardVector * Speed);
	ProjectileMovementComponent->Activate();
}

void AProjectile::OnHit(UPrimitiveComponent * HitComponent, AActor * OtherActor, UPrimitiveComponent * OtherComponent, FVector NormalImpulse, const FHitResult & Hit)
{
	// When a projectile hits something, turn off its launch blast trail
	this->LaunchBlast->Deactivate();

	// Then, add the impact blast effect
	this->ImpactBlast->Activate();

	// Fire the radial impulse
	ExplosionForce->FireImpulse();

	// Set the impact blast as the new root component
	SetRootComponent(this->ImpactBlast);

	// Destroy the collision mesh
	this->CollisionMesh->DestroyComponent();

	// Important: need to destroy the collision mesh before applying damage. Occasionally the collision mesh may block
	// damage from being applied to the tank.
	UGameplayStatics::ApplyRadialDamage(
		this,
		ProjectileDamage,
		GetActorLocation(),
		ExplosionForce->Radius, // For consistency i.e. if we change the ExplosionForce component it'll change here
		UDamageType::StaticClass(),
		TArray<AActor*>() // Damage all actors
	);

	// Start the despawn timer
	FTimerHandle Timer;
	GetWorld()->GetTimerManager().SetTimer(Timer, this, &AProjectile::OnTimerExpire, DestroyDelay, false);
}

void AProjectile::OnTimerExpire()
{
	// Despawn the projectile 
	Destroy();
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

