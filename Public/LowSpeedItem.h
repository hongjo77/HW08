#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "LowSpeedItem.generated.h"

UCLASS()
class HW08_API ALowSpeedItem : public ABaseItem
{
   GENERATED_BODY()

public:
   ALowSpeedItem();

protected:
   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
   float DebuffDuration = 5.0f;

   virtual void ActivateItem(AActor* Activator) override;
};
