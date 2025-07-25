#include "GameStateHW.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerControllerHW.h"
#include "SpawnVolume.h"
#include "GameInstanceHW.h"
#include "CoinItem.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "CharacterHW.h"
#include "MovingActor.h"
#include "RotatingActor.h"
#include "SpawnActor.h"

AGameStateHW::AGameStateHW()
{
	Score = 0;
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;
	LevelDuration = 30.0f;
	CurrentLevelIndex = 0;
	MaxLevels = 3;
}

void AGameStateHW::BeginPlay()
{
	Super::BeginPlay();

	StartLevel();

	GetWorldTimerManager().ClearTimer(HUDUpdateTimerHandle);
	GetWorldTimerManager().SetTimer(
		HUDUpdateTimerHandle,
		this,
		&AGameStateHW::UpdateHUD,
		0.1f,
		true
	);
}

int32 AGameStateHW::GetScore() const
{
	return Score;
}

void AGameStateHW::AddScore(int32 Amount)
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UGameInstanceHW* SpartaGameInstance = Cast<UGameInstanceHW>(GameInstance);
		if (SpartaGameInstance)
		{
			SpartaGameInstance->AddToScore(Amount);
		}
	}
	Score += Amount;
}

void AGameStateHW::OnGameOver()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (APlayerControllerHW* SpartaPlayerController = Cast<APlayerControllerHW>(PlayerController))
		{
			SpartaPlayerController->SetPause(true);
			SpartaPlayerController->ShowMainMenu(true);
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("game over"));
}

void AGameStateHW::OnCoinCollected()
{
	CollectedCoinCount++;
	UE_LOG(LogTemp, Warning, TEXT("Coin collected: %d / %d"),
		CollectedCoinCount,
		SpawnedCoinCount);
}

void AGameStateHW::StartLevel()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (APlayerControllerHW* SpartaPlayerController = Cast<APlayerControllerHW>(PlayerController))
		{
			SpartaPlayerController->ShowGameHUD();
		}
	}

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UGameInstanceHW* SpartaGameInstance = Cast<UGameInstanceHW>(GameInstance);
		if (SpartaGameInstance)
		{
			CurrentLevelIndex = SpartaGameInstance->CurrentLevelIndex;
		}
	}
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;

	TArray<AActor*> FoundVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes);

	const int32 ItemToSpawn = 10;
	for (int32 i = 0; i < ItemToSpawn; i++)
	{
		if (FoundVolumes.Num() > 0)
		{
			ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(FoundVolumes[0]);
			if (SpawnVolume)
			{
				AActor* SpawnedActor = SpawnVolume->SpawnRandomItem();
				if (SpawnedActor && SpawnedActor->IsA(ACoinItem::StaticClass()))
				{
					SpawnedCoinCount++;
				}
			}
		}
	}

	GetWorldTimerManager().SetTimer(
		LevelTimerHandle,
		this,
		&AGameStateHW::OnLevelTimeUp,
		LevelDuration,
		false
	);

	CurrentWaveIndex = 0;
    StartWave(); 
}

void AGameStateHW::StartWave()
{
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMovingActor::StaticClass(), FoundActors);
    for (AActor* Actor : FoundActors) Actor->Destroy();
    FoundActors.Empty();
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARotatingActor::StaticClass(), FoundActors);
    for (AActor* Actor : FoundActors) Actor->Destroy();

    int32 NumMoving = 3 + CurrentWaveIndex * 2;
    int32 NumRotating = 3 + CurrentWaveIndex * 2;

    TArray<AActor*> SpawnActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnActor::StaticClass(), SpawnActors);
    for (AActor* Actor : SpawnActors)
    {
        if (ASpawnActor* Spawner = Cast<ASpawnActor>(Actor))
        {
            Spawner->SpawnWave(NumMoving, NumRotating);
        }
    }

    GetWorldTimerManager().SetTimer(
        WaveTimerHandle,
        this,
        &AGameStateHW::OnWaveTimeUp,
        WaveDuration,
        false
    );
}

void AGameStateHW::OnWaveTimeUp()
{
    CurrentWaveIndex++;
    if (CurrentWaveIndex < MaxWaves)
    {
        StartWave();
    }
    else
    {
        EndLevel();
    }
}

void AGameStateHW::OnLevelTimeUp()
{
	CurrentWaveIndex++;
    if (CurrentWaveIndex < MaxWaves)
    {
        StartWave();
    }
    else
    {
        EndLevel();
    }
}

void AGameStateHW::EndLevel()
{
	// 1. HUD 타이머 먼저 꺼준다 (최상단 추가!)
	GetWorldTimerManager().ClearTimer(HUDUpdateTimerHandle);

	// 기존 코드 (레벨 타이머 해제)
	GetWorldTimerManager().ClearTimer(LevelTimerHandle);

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UGameInstanceHW* SpartaGameInstance = Cast<UGameInstanceHW>(GameInstance);
		if (SpartaGameInstance)
		{
			AddScore(Score);
			CurrentLevelIndex++;
			SpartaGameInstance->CurrentLevelIndex = CurrentLevelIndex;
		}
	}
	if (CurrentLevelIndex > MaxLevels)
	{
		OnGameOver();
		return;
	}
	if (LevelMapNames.IsValidIndex(CurrentLevelIndex))
	{
		UGameplayStatics::OpenLevel(GetWorld(), LevelMapNames[CurrentLevelIndex]);
	}
	else
	{
		OnGameOver();
	}
}


void AGameStateHW::UpdateHUD()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (APlayerControllerHW* SpartaPlayerController = Cast<APlayerControllerHW>(PlayerController))
		{
			if (UUserWidget* HUDWidget = SpartaPlayerController->GetHUDWidget())
			{
				if (UTextBlock* TimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Time"))))
				{
					//float RemainingTime = GetWorldTimerManager().GetTimerRemaining(LevelTimerHandle);
					float RemainingTime = GetWorldTimerManager().GetTimerRemaining(WaveTimerHandle);
					TimeText->SetText(FText::FromString(FString::Printf(TEXT("Time: %.1f"), RemainingTime)));
				}

				if (UTextBlock* ScoreText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Score"))))
				{
					if (UGameInstance* GameInstance = GetGameInstance())
					{
						UGameInstanceHW* SpartaGameInstance = Cast<UGameInstanceHW>(GameInstance);
						if (SpartaGameInstance)
						{
							ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), SpartaGameInstance->TotalScore)));
						}
					}
				}

				if (UTextBlock* LevelIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Level"))))
				{
					LevelIndexText->SetText(FText::FromString(FString::Printf(TEXT("Level: %d"), CurrentLevelIndex + 1)));
				}

				if (UTextBlock* WaveText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Wave"))))
				{
					WaveText->SetText(FText::FromString(FString::Printf(TEXT("Wave: %d"), CurrentWaveIndex + 1)));
				}

				UUserWidget* WBP_HP_Widget = Cast<UUserWidget>(HUDWidget->GetWidgetFromName(TEXT("WBP_HP")));
				if (WBP_HP_Widget)
				{
					UTextBlock* HPText = Cast<UTextBlock>(WBP_HP_Widget->GetWidgetFromName(TEXT("OverHeadHp")));
					if (HPText)
					{
						APawn* Pawn = PlayerController->GetPawn();
						if (ACharacterHW* Character = Cast<ACharacterHW>(Pawn))
						{
							int32 HP = Character->GetHealth();
							HPText->SetText(FText::FromString(FString::Printf(TEXT("HP : %d / 100"), HP)));
						}
					}
				}
			}
		}
	}
}


