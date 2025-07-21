#include "MovingActor.h"
#include "TimerManager.h"

AMovingActor::AMovingActor()
{
    PrimaryActorTick.bCanEverTick = true;
    StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
    RootComponent = StaticMesh;
}

void AMovingActor::BeginPlay()
{
    Super::BeginPlay();
    StartLocation = GetActorLocation();
    CurrentDirection = MoveDirection.GetSafeNormal();
    GetWorld()->GetTimerManager().SetTimer(TimerHandle_Destroy, this, &AMovingActor::DestroyByTimer, LifeTime, false);
}

void AMovingActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    FVector NewLocation = GetActorLocation() + CurrentDirection * MoveSpeed * DeltaTime;
    float Distance = FVector::Dist(StartLocation, NewLocation);
    if (Distance > MaxRange)
    {
        CurrentDirection *= -1;
        NewLocation = GetActorLocation() + CurrentDirection * MoveSpeed * DeltaTime;
    }
    SetActorLocation(NewLocation);
}

void AMovingActor::DestroyByTimer()
{
    Destroy();
}
