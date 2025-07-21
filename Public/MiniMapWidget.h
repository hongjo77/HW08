#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MiniMapWidget.generated.h"

UCLASS()
class HW08_API UMiniMapWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void UpdatePlayerOnMap(const FVector& PlayerWorldLoc, float PlayerYaw);

protected:
	UPROPERTY(meta=(BindWidget))
	class UImage* MinimapBaseImage;
	UPROPERTY(meta = (BindWidget))
	class UImage* PlayerIcon;
	
};
