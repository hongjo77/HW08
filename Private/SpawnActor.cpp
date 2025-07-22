#include "SpawnActor.h"
#include "Components/BoxComponent.h"
#include "MovingActor.h"
#include "RotatingActor.h"
#include "TimerManager.h"

ASpawnActor::ASpawnActor()
{
    PrimaryActorTick.bCanEverTick = false;
    SpawningBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawningBox"));
    SetRootComponent(SpawningBox);
}

void ASpawnActor::BeginPlay()
{
    Super::BeginPlay();
    for (int32 i = 0; i < NumMovingActors; ++i) SpawnOneMovingActor();
    for (int32 i = 0; i < NumRotatingActors; ++i) SpawnOneRotatingActor();
}

FVector ASpawnActor::GetRandomPointInVolume() const
{
    FVector Extent = SpawningBox->GetScaledBoxExtent();
    FVector Origin = SpawningBox->GetComponentLocation();
    return Origin + FVector(
        FMath::FRandRange(-Extent.X, Extent.X),
        FMath::FRandRange(-Extent.Y, Extent.Y),
        FMath::FRandRange(-Extent.Z, Extent.Z)
    );
}

void ASpawnActor::SpawnOneMovingActor()
{
    if (!MovingActorClass) return;
    FVector Loc = GetRandomPointInVolume();
    FVector Dir = FVector(
        FMath::FRandRange(-1.f, 1.f),
        FMath::FRandRange(-1.f, 1.f),
        0.f
    ).GetSafeNormal();
    float Speed = FMath::RandRange(100.f, 500.f);
    float Range = FMath::RandRange(200.f, 800.f);
    float Life = FMath::RandRange(MinLifeTime, MaxLifeTime);

    AMovingActor* NewActor = GetWorld()->SpawnActor<AMovingActor>(MovingActorClass, Loc, FRotator::ZeroRotator);
    if (NewActor)
    {
        NewActor->MoveSpeed = Speed;
        NewActor->MaxRange = Range;
        NewActor->MoveDirection = Dir;
        NewActor->LifeTime = Life;
        NewActor->OnDestroyed.AddDynamic(this, &ASpawnActor::OnActorDestroyed);
    }
}

void ASpawnActor::SpawnOneRotatingActor()
{
    if (!RotatingActorClass) return;
    FVector Loc = GetRandomPointInVolume();
    float YawSpeed = FMath::RandRange(60.f, 180.f);
    float Life = FMath::RandRange(MinLifeTime, MaxLifeTime);

    ARotatingActor* NewActor = GetWorld()->SpawnActor<ARotatingActor>(RotatingActorClass, Loc, FRotator::ZeroRotator);
    if (NewActor)
    {
        NewActor->RotationSpeed = FRotator(0, YawSpeed, 0);
        NewActor->LifeTime = Life;
        NewActor->OnDestroyed.AddDynamic(this, &ASpawnActor::OnActorDestroyed);
    }
}

void ASpawnActor::OnActorDestroyed(AActor* DestroyedActor)
{
    if (Cast<AMovingActor>(DestroyedActor))
    {
        FTimerHandle RespawnHandle;
        GetWorld()->GetTimerManager().SetTimer(
            RespawnHandle, this, &ASpawnActor::SpawnOneMovingActor, RespawnDelay, false
        );
    }
    else if (Cast<ARotatingActor>(DestroyedActor))
    {
        FTimerHandle RespawnHandle;
        GetWorld()->GetTimerManager().SetTimer(
            RespawnHandle, this, &ASpawnActor::SpawnOneRotatingActor, RespawnDelay, false
        );
    }
}

void ASpawnActor::SpawnWave(int32 NumMoving, int32 NumRotating)
{
    for (int32 i = 0; i < NumMoving; ++i) SpawnOneMovingActor();
    for (int32 i = 0; i < NumRotating; ++i) SpawnOneRotatingActor();
}