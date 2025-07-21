#include "RotatingActor.h"
#include "TimerManager.h"

ARotatingActor::ARotatingActor()
{
    PrimaryActorTick.bCanEverTick = true;
    StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
    RootComponent = StaticMesh;
}

void ARotatingActor::BeginPlay()
{
    Super::BeginPlay();
    GetWorld()->GetTimerManager().SetTimer(TimerHandle_Destroy, this, &ARotatingActor::DestroyByTimer, LifeTime, false);
}

void ARotatingActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    AddActorLocalRotation(RotationSpeed * DeltaTime);
}

void ARotatingActor::DestroyByTimer()
{
    Destroy();
}
