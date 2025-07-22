#include "CharacterHW.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "PlayerControllerHW.h"
#include "GameStateHW.h"
#include <Components/TextBlock.h>

#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"

ACharacterHW::ACharacterHW()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 300.0f;
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false;

	OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	OverheadWidget->SetupAttachment(GetMesh());
	OverheadWidget->SetWidgetSpace(EWidgetSpace::Screen);

	NormalSpeed = 600.0f;
	SprintSpeedMultiplier = 1.7f;
	SprintSpeed = NormalSpeed * SprintSpeedMultiplier;

	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;

	MaxHealth = 100.0f;
	Health = MaxHealth;
}

void ACharacterHW::BeginPlay()
{
	Super::BeginPlay();
	UpdateOverheadHP();
	
}
float ACharacterHW::GetHealth() const
{
	return Health;
}
void ACharacterHW::AddHealth(float Amount)
{
	Health = FMath::Clamp(Health + Amount, 0.0f, MaxHealth);
	UE_LOG(LogTemp, Warning, TEXT("heal"));
	UpdateOverheadHP();
}

void ACharacterHW::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInput = Cast< UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (APlayerControllerHW* PlayerController = Cast<APlayerControllerHW>(GetController()))
		{
			if (PlayerController->MoveAction)
			{
				EnhancedInput->BindAction(
					PlayerController->MoveAction,
					ETriggerEvent::Triggered,
					this,
					&ACharacterHW::Move
				);
			}

			if (PlayerController->JumpAction)
			{
				EnhancedInput->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Triggered,
					this,
					&ACharacterHW::StartJump
				);

				EnhancedInput->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Completed,
					this,
					&ACharacterHW::StopJump
				);
			}

			if (PlayerController->LookAction)
			{
				EnhancedInput->BindAction(
					PlayerController->LookAction,
					ETriggerEvent::Triggered,
					this,
					&ACharacterHW::Look
				);
			}

			if (PlayerController->SprintAction)
			{
				EnhancedInput->BindAction(
					PlayerController->SprintAction,
					ETriggerEvent::Triggered,
					this,
					&ACharacterHW::StartSprint
				);

				EnhancedInput->BindAction(
					PlayerController->SprintAction,
					ETriggerEvent::Completed,
					this,
					&ACharacterHW::StopSprint
				);
			}
		}
	}
}
float ACharacterHW::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Health = FMath::Clamp(Health - DamageAmount, 0.0f, MaxHealth);
	UpdateOverheadHP();
	UE_LOG(LogTemp, Warning, TEXT("damage"));
	if (Health <= 0.0f)
	{
		OnDeath();
	}

	return ActualDamage;
}

void ACharacterHW::Move(const FInputActionValue& value)
{
	if (!Controller) return;

	const FVector2D MoveInput = value.Get<FVector2D>();

	if (!FMath::IsNearlyZero(MoveInput.X))
	{
		AddMovementInput(GetActorForwardVector(), MoveInput.X);
	}

	if (!FMath::IsNearlyZero(MoveInput.Y))
	{
		AddMovementInput(GetActorRightVector(), MoveInput.Y);
	}
}

void ACharacterHW::StartJump(const FInputActionValue& value)
{
	if (value.Get<bool>())
	{
		Jump();
	}
}

void ACharacterHW::StopJump(const FInputActionValue& value)
{
	if (!value.Get<bool>())
	{
		StopJumping();
	}
}

void ACharacterHW::Look(const FInputActionValue& value)
{
	const FVector2D LookInput = value.Get<FVector2D>();

	AddControllerYawInput(LookInput.X);
	AddControllerPitchInput(LookInput.Y);
}

void ACharacterHW::StartSprint(const FInputActionValue& value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
}

void ACharacterHW::StopSprint(const FInputActionValue& value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	}
}

void ACharacterHW::OnDeath()
{
	AGameStateHW* SpartaGameState = GetWorld() ? GetWorld()->GetGameState<AGameStateHW>() : nullptr;
	if (SpartaGameState)
	{
		SpartaGameState->OnGameOver();
	}
}

void ACharacterHW::UpdateOverheadHP()
{
	if (!OverheadWidget) return;

	UUserWidget* OverheadWidgetInstance = OverheadWidget->GetUserWidgetObject();
	if (!OverheadWidgetInstance) return;

	if (UTextBlock* HPText = Cast<UTextBlock>(OverheadWidgetInstance->GetWidgetFromName(TEXT("OverHeadHP"))))
	{
		HPText->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %0.f"), Health, MaxHealth)));
	}
}

void ACharacterHW::ApplyLowSpeedDebuff(float Duration)
{
	// 누적 지속시간 관리
	LowSpeedDebuffRemaining += Duration;

	// 속도 감소 적용
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed * 0.5f; // 50% 속도 감소
	}

	// Debuff 텍스트 표시
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC)
	{
		UUserWidget* HUDWidget = nullptr;
		if (PC->IsA(APlayerControllerHW::StaticClass()))
		{
			HUDWidget = Cast<APlayerControllerHW>(PC)->GetHUDWidget();
		}
		if (HUDWidget)
		{
			if (UTextBlock* DebuffText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Debuff"))))
			{
				DebuffText->SetText(FText::FromString(TEXT("Debuff: Low Speed")));
				DebuffText->SetVisibility(ESlateVisibility::Visible);
			}
		}
	}

	// 타이머 갱신
	GetWorldTimerManager().ClearTimer(LowSpeedDebuffTimerHandle);
	GetWorldTimerManager().SetTimer(
		LowSpeedDebuffTimerHandle,
		[this]()
		{
			LowSpeedDebuffRemaining = 0.f;
			// 속도 복구
			if (GetCharacterMovement())
			{
				GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
			}
			// Debuff 텍스트 숨김
			APlayerController* PC2 = UGameplayStatics::GetPlayerController(GetWorld(), 0);
			if (PC2)
			{
				UUserWidget* HUDWidget2 = nullptr;
				if (PC2->IsA(APlayerControllerHW::StaticClass()))
				{
					HUDWidget2 = Cast<APlayerControllerHW>(PC2)->GetHUDWidget();
				}
				if (HUDWidget2)
				{
					if (UTextBlock* DebuffText2 = Cast<UTextBlock>(HUDWidget2->GetWidgetFromName(TEXT("Debuff"))))
					{
						DebuffText2->SetVisibility(ESlateVisibility::Hidden);
					}
				}
			}
		},
		LowSpeedDebuffRemaining,
		false
	);
}
