// Copyright Epic Games, Inc. All Rights Reserved.

#include "AventuraUSFX022026L1Pawn.h"
#include "AventuraUSFX022026L1Projectile.h"
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/CollisionProfile.h"
#include "Engine/StaticMesh.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"

const FName AAventuraUSFX022026L1Pawn::MoveForwardBinding("MoveForward");
const FName AAventuraUSFX022026L1Pawn::MoveRightBinding("MoveRight");
const FName AAventuraUSFX022026L1Pawn::FireForwardBinding("FireForward");
const FName AAventuraUSFX022026L1Pawn::FireRightBinding("FireRight");

AAventuraUSFX022026L1Pawn::AAventuraUSFX022026L1Pawn()
{	

	//////ACA SE CAMBIA LA MALLA DEL PAWN
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ShipMesh(
		TEXT("/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube")
	);

	// Create the mesh component
	ShipMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	RootComponent = ShipMeshComponent;

	ShipMeshComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	ShipMeshComponent->SetStaticMesh(ShipMesh.Object);

	// Escala de la plataforma/nav
	ShipMeshComponent->SetWorldScale3D(FVector(4.0f, 1.5f, 2.0f));

	////////------------------------
	
	// Cache our sound effect
	static ConstructorHelpers::FObjectFinder<USoundBase> FireAudio(TEXT("/Game/TwinStick/Audio/TwinStickFire.TwinStickFire"));
	FireSound = FireAudio.Object;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when ship does
	CameraBoom->TargetArmLength = 1200.f;
	CameraBoom->SetRelativeRotation(FRotator(-80.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	CameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;	// Camera does not rotate relative to arm

	// Movement
	MoveSpeed = 1000.0f;
	// Weapon
	GunOffset = FVector(90.f, 0.f, 0.f);
	FireRate = 0.1f;
	bCanFire = true;
}

void AAventuraUSFX022026L1Pawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	// set up gameplay key bindings
	PlayerInputComponent->BindAxis(MoveForwardBinding);
	PlayerInputComponent->BindAxis(MoveRightBinding);
	PlayerInputComponent->BindAxis(FireForwardBinding);
	PlayerInputComponent->BindAxis(FireRightBinding);
}

void AAventuraUSFX022026L1Pawn::Tick(float DeltaSeconds)
{
	// Movimiento únicamente de izquierda a derecha
	const float RightValue = GetInputAxisValue(MoveRightBinding);

	const FVector MoveDirection = FVector(0.f, RightValue, 0.f);

	// Calcular movimiento
	const FVector Movement = MoveDirection * MoveSpeed * DeltaSeconds;

	///////////----------------------------

	// If non-zero size, move this actor
	if (Movement.SizeSquared() > 0.0f)
	{
		const FRotator NewRotation = GetActorRotation();
		FHitResult Hit(1.f);
		RootComponent->MoveComponent(Movement, NewRotation, true, &Hit);
		
		if (Hit.IsValidBlockingHit())
		{
			const FVector Normal2D = Hit.Normal.GetSafeNormal2D();
			const FVector Deflection = FVector::VectorPlaneProject(Movement, Normal2D) * (1.f - Hit.Time);
			RootComponent->MoveComponent(Deflection, NewRotation, true);
		}
	}
	
	// Create fire direction vector
	const float FireForwardValue = GetInputAxisValue(FireForwardBinding);
	const float FireRightValue = GetInputAxisValue(FireRightBinding);
	const FVector FireDirection = FVector(FireForwardValue, FireRightValue, 0.f);

	// Try and fire a shot
	FireShot(FireDirection);
}

void AAventuraUSFX022026L1Pawn::FireShot(FVector FireDirection)
{
	// If it's ok to fire again
	if (bCanFire == true)
	{
		// If we are pressing fire stick in a direction
		if (FireDirection.SizeSquared() > 0.0f)
		{

			//Esto hará que cada disparo tenga una pequeña variación aleatoria en X e Y, por lo que la pelota no seguirá exactamente la misma línea.
			FVector Direccion = FireDirection.GetSafeNormal();

			float Variacion = FMath::RandRange(-0.35f, 0.35f);

			Direccion = FVector(
				Direccion.X + Variacion,
				Direccion.Y + FMath::RandRange(-0.35f, 0.35f),
				0.0f
			).GetSafeNormal();

			const FRotator FireRotation = Direccion.Rotation();

			// Spawn projectile at an offset from this pawn
			const FVector SpawnLocation = GetActorLocation() + FireRotation.RotateVector(GunOffset);

			UWorld* const World = GetWorld();
			if (World != nullptr)
			{
				// spawn the projectile
				World->SpawnActor<AAventuraUSFX022026L1Projectile>(SpawnLocation, FireRotation);
			}

			bCanFire = false;
			World->GetTimerManager().SetTimer(TimerHandle_ShotTimerExpired, this, &AAventuraUSFX022026L1Pawn::ShotTimerExpired, FireRate);

			// try and play the sound if specified
			if (FireSound != nullptr)
			{
				UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
			}

			bCanFire = false;
		}
	}
}

void AAventuraUSFX022026L1Pawn::ShotTimerExpired()
{
	bCanFire = true;
}

