// Copyright Epic Games, Inc. All Rights Reserved.

#include "AventuraUSFX022026L1Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"

AAventuraUSFX022026L1Projectile::AAventuraUSFX022026L1Projectile()
{
	// Mesh de la pelota
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ProjectileMeshAsset(
		TEXT("/Game/TwinStick/Meshes/TwinStickProjectile.TwinStickProjectile")
	);

	// Crear componente de malla
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh0"));

	ProjectileMesh->SetStaticMesh(ProjectileMeshAsset.Object);
	ProjectileMesh->SetRelativeScale3D(FVector(3.0f, 3.0f, 3.0f));

	// Configuración de colisión
	ProjectileMesh->BodyInstance.SetCollisionProfileName("Projectile");

	// Detectar impactos
	ProjectileMesh->OnComponentHit.AddDynamic(
		this,
		&AAventuraUSFX022026L1Projectile::OnHit
	);

	RootComponent = ProjectileMesh;

	// Componente de movimiento
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(
		TEXT("ProjectileMovement0")
	);

	ProjectileMovement->UpdatedComponent = ProjectileMesh;

	// Velocidad de la pelota
	ProjectileMovement->InitialSpeed = 3500.f;
	ProjectileMovement->MaxSpeed = 3500.f;

	ProjectileMovement->Bounciness = 1.0f;
	ProjectileMovement->Friction = 0.0f;
	ProjectileMovement->BounceVelocityStopSimulatingThreshold = 0.0f;

	// La pelota sigue la dirección de movimiento
	ProjectileMovement->bRotationFollowsVelocity = true;

	// ACTIVAR REBOTE
	ProjectileMovement->bShouldBounce = true;

	// Sin gravedad
	ProjectileMovement->ProjectileGravityScale = 0.f;

	// Mantener la pelota únicamente en el plano X-Y
	ProjectileMovement->bConstrainToPlane = true;
	ProjectileMovement->SetPlaneConstraintNormal(FVector(0.f, 0.f, 1.f));

	// La pelota NO desaparece automáticamente
	InitialLifeSpan = 0.0f;
}

void AAventuraUSFX022026L1Projectile::OnHit(
	UPrimitiveComponent* HitComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	FVector NormalImpulse,
	const FHitResult& Hit)
{
	// La pelota NO se destruye.
	// El ProjectileMovementComponent se encarga del rebote.

	if ((OtherActor != nullptr) &&
		(OtherActor != this) &&
		(OtherComp != nullptr) &&
		OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(
			GetVelocity() * 20.0f,
			GetActorLocation()
		);
	}

	// NO colocar Destroy() aquí.
}