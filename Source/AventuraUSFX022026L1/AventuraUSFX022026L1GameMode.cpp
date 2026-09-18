// Copyright Epic Games, Inc. All Rights Reserved.

#include "AventuraUSFX022026L1GameMode.h"
#include "AventuraUSFX022026L1Pawn.h"
#include "PlataformaIndestructible.h"
#include "PlataformaDestructible.h"

AAventuraUSFX022026L1GameMode::AAventuraUSFX022026L1GameMode()
{
	DefaultPawnClass = AAventuraUSFX022026L1Pawn::StaticClass();

	tipoPlataformaActual = 0;
}

void AAventuraUSFX022026L1GameMode::BeginPlay()
{
    Super::BeginPlay();

    GetWorldTimerManager().SetTimer(
        TimerSpawnPlataforma,
        this,
        &AAventuraUSFX022026L1GameMode::SpawnPlataforma,
        5.0f,
        true,
        5.0f
    );
}

void AAventuraUSFX022026L1GameMode::SpawnPlataforma()
{
    UWorld* World = GetWorld();

    if (World == nullptr)
    {
        return;
    }

    FRotator SpawnRotation = FRotator::ZeroRotator;

    // ÁREA 1
    float Y1 = FMath::RandRange(-1600.0f, -1200.0f);

    FVector SpawnLocation1 = FVector(
        1800.0f,
        Y1,
        250.0f
    );

    if (FMath::RandRange(0, 1) == 0)
    {
        World->SpawnActor<APlataformaIndestructible>(
            SpawnLocation1,
            SpawnRotation
        );
    }
    else
    {
        World->SpawnActor<APlataformaDestructible>(
            SpawnLocation1,
            SpawnRotation
        );
    }


    // ÁREA 2
    float Y2 = FMath::RandRange(-300.0f, 300.0f);

    FVector SpawnLocation2 = FVector(
        1800.0f,
        Y2,
        250.0f
    );

    if (FMath::RandRange(0, 1) == 0)
    {
        World->SpawnActor<APlataformaIndestructible>(
            SpawnLocation2,
            SpawnRotation
        );
    }
    else
    {
        World->SpawnActor<APlataformaDestructible>(
            SpawnLocation2,
            SpawnRotation
        );
    }


    // ÁREA 3
    float Y3 = FMath::RandRange(1200.0f, 1600.0f);

    FVector SpawnLocation3 = FVector(
        1800.0f,
        Y3,
        250.0f
    );

    if (FMath::RandRange(0, 1) == 0)
    {
        World->SpawnActor<APlataformaIndestructible>(
            SpawnLocation3,
            SpawnRotation
        );
    }
    else
    {
        World->SpawnActor<APlataformaDestructible>(
            SpawnLocation3,
            SpawnRotation
        );
    }
}

void AAventuraUSFX022026L1GameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAventuraUSFX022026L1GameMode::EliminarPlataforma()
{
}