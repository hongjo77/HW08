#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RotatingActor.generated.h"

UCLASS(Blueprintable)
class HW08_API ARotatingActor : public AActor
{
    GENERATED_BODY()

public:
    ARotatingActor();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
    UStaticMeshComponent* StaticMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotate")
    FRotator RotationSpeed = FRotator(0, 90, 0);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Life")
    float LifeTime = 20.f;

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

private:
    FTimerHandle TimerHandle_Destroy;

    void DestroyByTimer();
};
