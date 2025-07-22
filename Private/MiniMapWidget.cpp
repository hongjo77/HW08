#include "MiniMapWidget.h"
#include "Components/Image.h"

void UMiniMapWidget::SetCurrentLevelIndex(int32 Index)
{
	CurrentMapIndex = Index;
	const FMiniMapInfo* Info = GetCurrentMapInfo();
	if (Info && Info->MapTexture && MinimapBaseImage)
	{
		FSlateBrush Brush;
		Brush.SetResourceObject(Info->MapTexture);
		MinimapBaseImage->SetBrush(Brush);
	}
}

void UMiniMapWidget::UpdateMiniMap(const FVector& PlayerWorldLoc, float PlayerYaw)
{
	if (!MinimapBaseImage || !PlayerIcon) return;
	const FMiniMapInfo* Info = GetCurrentMapInfo();
	if (!Info || !Info->MapTexture) return;

	// 미니맵에서 보여줄 윈도우 영역 계산
	float HalfView = MiniMapVisibleWorldSize * 0.5f;
	FVector2D WorldPlayer(PlayerWorldLoc.X, PlayerWorldLoc.Y);

	// 플레이어의 월드상의 상대 이동량 계산
	FVector2D DeltaWorld = WorldPlayer - Info->PlayerStartWorld;

	// 미니맵 이미지에서 픽셀 단위 거리 계산
	FVector2D TextureSize = FVector2D(
		Info->MapTexture->GetSizeX(),
		Info->MapTexture->GetSizeY()
	);
	FVector2D PixelDelta;
	PixelDelta.X = (DeltaWorld.X / Info->MapWorldSize.X) * TextureSize.X;
	PixelDelta.Y = (DeltaWorld.Y / Info->MapWorldSize.Y) * TextureSize.Y;

	// 실제 이미지상의 위치 = 시작 픽셀 + 상대 픽셀 이동량
	FVector2D PlayerIconPixel = Info->PlayerStartUV + PixelDelta;

	// 미니맵에서 윈도우 보여줄 좌상단 픽셀 위치 계산
	FVector2D WindowMin = PlayerIconPixel - FVector2D(
		(MiniMapWidgetSize.X / 2.f) * (TextureSize.X / MiniMapWidgetSize.X),
		(MiniMapWidgetSize.Y / 2.f) * (TextureSize.Y / MiniMapWidgetSize.Y)
	);

	// 배경 이미지를 반대로 offset(스크롤 효과)
	MinimapBaseImage->SetRenderTranslation(-WindowMin * (MiniMapWidgetSize / TextureSize));

	// 플레이어 아이콘은 미니맵 위젯의 중앙에 고정
	PlayerIcon->SetRenderTranslation(FVector2D(MiniMapWidgetSize.X / 2.f, MiniMapWidgetSize.Y / 2.f));

	PlayerIcon->SetRenderTransformAngle(PlayerYaw);
}