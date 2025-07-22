#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerControllerHW.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS(Blueprintable)
class HW08_API APlayerControllerHW : public APlayerController
{
	GENERATED_BODY()
	
public:
	APlayerControllerHW();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* InputmappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* SprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<UUserWidget> HUDWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HUD")
	UUserWidget* HUDWidgetInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Menu")
	UUserWidget* MainMenuWidgetInstance;

	UFUNCTION(BlueprintPure, Category = "HUD")
	UUserWidget* GetHUDWidget() const;

	UPROPERTY()
	class UMiniMapWidget* MiniMapWidget; // MiniMap BP를 부모로 한 위젯
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> MiniMapWidgetClass;

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ShowGameHUD();
	UFUNCTION(BlueprintCallable, Category = "Menu")
	void ShowMainMenu(bool bIsRestart);
	UFUNCTION(BlueprintCallable, Category = "Menu")
	void StartGame();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
};
