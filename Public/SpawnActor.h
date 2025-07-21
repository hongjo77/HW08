#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnActor.generated.h"

class UBoxComponent;

UCLASS(Blueprintable)
class HW08_API ASpawnActor : public AActor
{
    GENERATED_BODY()

public:
    ASpawnActor();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, Category = "Spawn")
    UBoxComponent* SpawningBox;

    UPROPERTY(EditAnywhere, Category = "Spawn")
    TSubclassOf<class AMovingActor> MovingActorClass;

    UPROPERTY(EditAnywhere, Category = "Spawn")
    TSubclassOf<class ARotatingActor> RotatingActorClass;

    UPROPERTY(EditAnywhere, Category = "Spawn")
    int32 NumMovingActors = 3;

    UPROPERTY(EditAnywhere, Category = "Spawn")
    int32 NumRotatingActors = 3;

    UPROPERTY(EditAnywhere, Category = "Spawn")
    float MinLifeTime = 20.f;
    UPROPERTY(EditAnywhere, Category = "Spawn")
    float MaxLifeTime = 30.f;

    UPROPERTY(EditAnywhere, Category = "Spawn")
    float RespawnDelay = 2.f;

    void SpawnOneMovingActor();
    void SpawnOneRotatingActor();
    FVector GetRandomPointInVolume() const;

    UFUNCTION()
    void OnActorDestroyed(AActor* DestroyedActor);
};
