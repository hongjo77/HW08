#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterHW.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UWidgetComponent;
struct FInputActionValue;

UCLASS(Blueprintable)
class HW08_API ACharacterHW : public ACharacter
{
	GENERATED_BODY()

public:
	ACharacterHW();
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* OverheadWidget;

	UFUNCTION(BlueprintPure, Category = "Health")
	float GetHealth() const;
	UFUNCTION(BlueprintCallable, Category = "Health")
	void AddHealth(float Amount);

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float Health;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent, // 불속성, 물속성 이런거 할수있음
		AController* EventInstigator, // 지뢰를 심은 사람
		AActor* DamageCauser) override; // 데미지를 일으킨 액터

	UFUNCTION()
	void Move(const FInputActionValue& value);
	UFUNCTION()
	void StartJump(const FInputActionValue& value);
	UFUNCTION()
	void StopJump(const FInputActionValue& value);
	UFUNCTION()
	void Look(const FInputActionValue& value);
	UFUNCTION()
	void StartSprint(const FInputActionValue& value);
	UFUNCTION()
	void StopSprint(const FInputActionValue& value);

	void OnDeath();
	void UpdateOverheadHP();

private:
   float NormalSpeed;
   float SprintSpeedMultiplier;
   float SprintSpeed;

   FTimerHandle LowSpeedDebuffTimerHandle;
   float LowSpeedDebuffRemaining = 0.f;

public:
   void ApplyLowSpeedDebuff(float Duration);

};
