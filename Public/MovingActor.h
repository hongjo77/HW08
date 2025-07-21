#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovingActor.generated.h"

UCLASS(Blueprintable)
class HW08_API AMovingActor : public AActor
{
    GENERATED_BODY()

public:
    AMovingActor();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
    UStaticMeshComponent* StaticMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
    float MoveSpeed = 200.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
    float MaxRange = 300.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
    FVector MoveDirection = FVector(1, 0, 0);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Life")
    float LifeTime = 20.f;

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

private:
    FVector StartLocation;
    FVector CurrentDirection;
    FTimerHandle TimerHandle_Destroy;

    void DestroyByTimer();
};
