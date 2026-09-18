#pragma once

#include "CoreMinimal.h"
#include "Plataforma.h"
#include "PlataformaIndestructible.generated.h"

UCLASS()
class AVENTURAUSFX022026L1_API APlataformaIndestructible : public APlataforma
{
    GENERATED_BODY()

public:
    APlataformaIndestructible();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    UFUNCTION()
    void OnHitIndestructible(
        UPrimitiveComponent* HitComp,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        FVector NormalImpulse,
        const FHitResult& Hit
    );
};