#include "PlataformaDestructible.h"
#include "AventuraUSFX022026L1Projectile.h"
#include "Components/StaticMeshComponent.h"
//se añade para materiales 
#include "Materials/MaterialInstanceDynamic.h"
//
#include "Materials/MaterialInterface.h"
#include "UObject/ConstructorHelpers.h"

APlataformaDestructible::APlataformaDestructible()
{
    if (mallaPlataforma)
    {
        mallaPlataforma->SetCollisionEnabled(
            ECollisionEnabled::QueryAndPhysics
        );

        mallaPlataforma->SetCollisionResponseToAllChannels(ECR_Block);

        mallaPlataforma->OnComponentHit.RemoveDynamic(
            this,
            &APlataforma::OnHit
        );

        mallaPlataforma->OnComponentHit.AddDynamic(
            this,
            &APlataformaDestructible::OnHitDestructible
        );
    }

    //material
    static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialAsset(
        TEXT("/Game/StarterContent/Materials/M_Ground_Moss.M_Ground_Moss")
    );

    if (MaterialAsset.Succeeded())
    {
        mallaPlataforma->SetMaterial(0, MaterialAsset.Object);
    }
}



//Esto hará que las indestructibles sean rojos.
void APlataformaDestructible::BeginPlay()
{
    Super::BeginPlay();

    UMaterialInstanceDynamic* MaterialDinamico =
        mallaPlataforma->CreateAndSetMaterialInstanceDynamic(0);

    if (MaterialDinamico)
    {
        MaterialDinamico->SetVectorParameterValue(
            "BaseColor",
            FLinearColor(1.0f, 0.0f, 0.0f, 1.0f)
        );
    }
}

void APlataformaDestructible::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void APlataformaDestructible::OnHitDestructible(
    UPrimitiveComponent* HitComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    FVector NormalImpulse,
    const FHitResult& Hit
)
{
    if (OtherActor &&
        OtherActor->IsA(AAventuraUSFX022026L1Projectile::StaticClass()))
    {
        Destroy();
    }
}