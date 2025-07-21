#include "GameInstanceHW.h"

UGameInstanceHW::UGameInstanceHW()
{
	TotalScore = 0;
	CurrentLevelIndex = 0;
}

void UGameInstanceHW::AddToScore(int32 Amount)
{
	TotalScore += Amount;
	UE_LOG(LogTemp, Warning, TEXT("total score %d"), TotalScore);
}
