#include "PlataformaIndestructible.h"
#include "AventuraUSFX022026L1Pawn.h"
#include "Components/StaticMeshComponent.h"
//se añade para materiales 
#include "Materials/MaterialInstanceDynamic.h"
//
#include "Materials/MaterialInterface.h"
#include "UObject/ConstructorHelpers.h"

APlataformaIndestructible::APlataformaIndestructible()
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
            &APlataformaIndestructible::OnHitIndestructible
        );
    }

    //material
    static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialAsset(
        TEXT("/Game/StarterContent/Materials/M_Metal_Gold.M_Metal_Gold")
    );

    if (MaterialAsset.Succeeded())
    {
        mallaPlataforma->SetMaterial(0, MaterialAsset.Object);
    }
}




//Esto hará que las indestructibles sean azules.
void APlataformaIndestructible::BeginPlay()
{
    Super::BeginPlay();

    UMaterialInstanceDynamic* MaterialDinamico =
        mallaPlataforma->CreateAndSetMaterialInstanceDynamic(0);

    if (MaterialDinamico)
    {
        MaterialDinamico->SetVectorParameterValue(
            "BaseColor",
            FLinearColor(0.0f, 0.2f, 1.0f, 1.0f)
        );
    }
}

void APlataformaIndestructible::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void APlataformaIndestructible::OnHitIndestructible(
    UPrimitiveComponent* HitComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    FVector NormalImpulse,
    const FHitResult& Hit
)
{
    if (OtherActor &&
        OtherActor->IsA(AAventuraUSFX022026L1Pawn::StaticClass()))
    {
        Destroy();
    }
}