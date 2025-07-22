#include "LowSpeedItem.h"
#include "CharacterHW.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "PlayerControllerHW.h"

ALowSpeedItem::ALowSpeedItem()
{
    DebuffDuration = 5.0f;
    ItemType = "Debuff";
}

void ALowSpeedItem::ActivateItem(AActor* Activator)
{
    if (ACharacterHW* Character = Cast<ACharacterHW>(Activator))
    {
        // Debuff: 속도 감소
        Character->ApplyLowSpeedDebuff(DebuffDuration);

        // Debuff 텍스트 표시
        if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
        {
            UUserWidget* HUDWidget = nullptr;
            if (APlayerControllerHW* CustomPC = Cast<APlayerControllerHW>(PC))
            {
                HUDWidget = CustomPC->GetHUDWidget();
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
    }
}

