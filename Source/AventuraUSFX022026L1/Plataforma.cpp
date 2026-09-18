#include "Plataforma.h"
#include "AventuraUSFX022026L1Projectile.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"

APlataforma::APlataforma()
{
    PrimaryActorTick.bCanEverTick = true;

    //forma de la plataforma
    static ConstructorHelpers::FObjectFinder<UStaticMesh> PlataformaMeshAsset(
        TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'")
    );

    mallaPlataforma = CreateDefaultSubobject<UStaticMeshComponent>(
        TEXT("MallaPlataforma")
    );

    mallaPlataforma->SetStaticMesh(PlataformaMeshAsset.Object);

    //escala de la plataforma
    RootComponent = mallaPlataforma;
    mallaPlataforma->SetRelativeScale3D(FVector(1.0f, 4.5f, 2.0f));

    mallaPlataforma->SetCollisionEnabled(
        ECollisionEnabled::QueryAndPhysics
    );

    mallaPlataforma->SetCollisionResponseToAllChannels(
        ECR_Block
    );

    mallaPlataforma->OnComponentHit.AddDynamic(
        this,
        &APlataforma::OnHit
    );

    movimientoEjes = FVector(1.0f, 0.0f, 0.0f);

    movimientoLimitesMaximos = FVector(
        FMath::RandRange(200.0f, 800.0f),
        FMath::RandRange(200.0f, 800.0f),
        FMath::RandRange(200.0f, 800.0f)
    );

    movimientoLimitesMinimos = FVector(
        FMath::RandRange(-800.0f, -200.0f),
        FMath::RandRange(-800.0f, -200.0f),
        FMath::RandRange(-800.0f, -200.0f)
    );

    movimientoVelocidades = FVector(
        FMath::RandRange(50.0f, 200.0f),
        FMath::RandRange(50.0f, 200.0f),
        FMath::RandRange(50.0f, 200.0f)
    );

    movimientoDireccion = FVector(
        FMath::RandRange(-1.0f, 1.0f),
        FMath::RandRange(-1.0f, 1.0f),
        FMath::RandRange(-1.0f, 1.0f)
    );

    TiempoTranscurrido = 0.0f;

    EstaCayendo = true;
    EstaApilada = false;
}


void APlataforma::BeginPlay()
{
    Super::BeginPlay();
}


void APlataforma::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    TiempoTranscurrido += DeltaTime;

    if (EstaCayendo)
    {
        FVector Posicion = GetActorLocation();

        // Caída en dirección X negativa
        Posicion.X -= 200.0f * DeltaTime;

        SetActorLocation(Posicion, true);

        return;
    }

    // Si ya está apilada, permanece quieta
}


void APlataforma::OnHit(
    UPrimitiveComponent* HitComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    FVector NormalImpulse,
    const FHitResult& Hit
)
{
    if (!EstaCayendo)
    {
        return;
    }

    if (OtherActor &&
        OtherActor->IsA(AAventuraUSFX022026L1Projectile::StaticClass()))
    {
        return;
    }

    EstaCayendo = false;
    EstaApilada = true;
}