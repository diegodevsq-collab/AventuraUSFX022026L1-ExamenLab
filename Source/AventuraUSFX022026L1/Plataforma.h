#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Plataforma.generated.h"

class UStaticMeshComponent;

enum class ETipoPlataforma
{
    PLATAFORMA_AEREA,
    PLATAFORMA_TERRESTRE,
    PLATAFORMA_SUBTERRANEA,
    PLATAFORMA_ACUATICA
};

UCLASS()
class AVENTURAUSFX022026L1_API APlataforma : public AActor
{
    GENERATED_BODY()

public:
    APlataforma();

    UFUNCTION()
    void OnHit(
        UPrimitiveComponent* HitComp,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        FVector NormalImpulse,
        const FHitResult& Hit
    );

    ETipoPlataforma tipoPlataforma;

    UStaticMeshComponent* mallaPlataforma;

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    FVector movimientoEjes = FVector(0.0f, 0.0f, 0.0f);
    FVector movimientoLimitesMaximos = FVector(0.0f, 0.0f, 0.0f);
    FVector movimientoLimitesMinimos = FVector(0.0f, 0.0f, 0.0f);
    FVector movimientoVelocidades = FVector(0.0f, 0.0f, 0.0f);
    FVector movimientoDireccion = FVector(0.0f, 0.0f, 0.0f);
    FVector posicionActual = FVector(0.0f, 0.0f, 0.0f);

    bool EstaCayendo;
    bool EstaApilada;

    float TiempoTranscurrido;
};