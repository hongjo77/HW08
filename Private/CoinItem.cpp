#include "CoinItem.h"
#include "Engine/World.h"
#include "GameStateHW.h"

ACoinItem::ACoinItem()
{
	PointValue = 50;
	ItemType = "Coin";
}

void ACoinItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);
	if (Activator && Activator->ActorHasTag("Player"))
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			2.0f,
			FColor::Green,
			FString::Printf(TEXT("Player Gained %d points!"), PointValue));

		if (UWorld* World = GetWorld())
		{
			if (AGameStateHW* GameState = World->GetGameState<AGameStateHW>())
			{
				GameState->AddScore(PointValue);
				GameState->OnCoinCollected();
			}
		}

		DestroyItem();
	}
}


