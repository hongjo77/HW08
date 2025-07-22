#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MiniMapWidget.generated.h"

USTRUCT(BlueprintType)
struct FMiniMapInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* MapTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D MapOrigin; // 월드 좌표 원점
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D MapWorldSize; // 월드 크기(가로,세로)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D PlayerStartWorld; // 월드의 플레이어 스타트 좌표
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D PlayerStartUV; // 미니맵 이미지 내 플레이어 스타트 (픽셀)
};

UCLASS()
class HW08_API UMiniMapWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetCurrentLevelIndex(int32 Index);

	UFUNCTION(BlueprintCallable)
	void UpdateMiniMap(const FVector& PlayerWorldLoc, float PlayerYaw);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MiniMap")
	TArray<FMiniMapInfo> MapInfos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MiniMap")
	FVector2D MiniMapWidgetSize = FVector2D(200.f, 200.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MiniMap")
	float MiniMapVisibleWorldSize = 20.f; // 미니맵에서 보여줄 월드 길이

protected:
	UPROPERTY(meta = (BindWidget))
	class UImage* MinimapBaseImage;

	UPROPERTY(meta = (BindWidget))
	class UImage* PlayerIcon;

private:
	int32 CurrentMapIndex = 0;

	FORCEINLINE const FMiniMapInfo* GetCurrentMapInfo() const
	{
		return MapInfos.IsValidIndex(CurrentMapIndex) ? &MapInfos[CurrentMapIndex] : nullptr;
	}
};