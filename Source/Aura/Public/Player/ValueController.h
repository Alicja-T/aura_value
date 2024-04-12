// Copyright Philosophical Games

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "ValueController.generated.h"

struct FInputActionValue;
class IEnemyInterface;
class UValueAbilitySystemComponent;
class USplineComponent;
    /**
 * 
 */
UCLASS()
class AURA_API AValueController : public APlayerController
{
	GENERATED_BODY()
 public:
  AValueController();
  virtual void PlayerTick(float DeltaTime) override;
 protected:
  virtual void BeginPlay() override;
  virtual void SetupInputComponent() override;

 private:
  UPROPERTY(EditAnywhere, Category = Input)
  TObjectPtr<class UInputMappingContext> ValueContext;
  UPROPERTY(EditAnywhere, Category = Input)
  TObjectPtr<class UInputAction> MoveAction;
  UPROPERTY(EditAnywhere, Category = Input)
  TObjectPtr<class UInputAction> LeftShiftAction;

  void LeftShiftPressed() { bLeftShiftDown = true; }
  void LeftShiftReleased() { bLeftShiftDown = false; }
  bool bLeftShiftDown = false;

  void Move(const FInputActionValue& InputActionValue);
  void CursorTrace();

  IEnemyInterface* LastActor;
  IEnemyInterface* CurrentActor;
  FHitResult CursorHit;

  void AbilityInputTagPressed(FGameplayTag InputTag);
  void AbilityInputTagReleased(FGameplayTag InputTag);
  void AbilityInputTagHeld(FGameplayTag AInputTag);


  UPROPERTY(EditDefaultsOnly, Category = Input)
  TObjectPtr<class UValueInputConfig> InputConfig;

  UPROPERTY()
  TObjectPtr<UValueAbilitySystemComponent> ValueAbilitySystemComponent;

  UValueAbilitySystemComponent* GetASC();

  FVector CachedDestination = FVector::ZeroVector;
  float FollowTime = 0.f;
  float ShortPressThreshold = 0.5f;
  bool bAutoRunning = false;
  bool bTargeting = false;

  UPROPERTY(EditDefaultsOnly)
  float AutoRunAcceptanceRadius = 50.f;

  UPROPERTY(VisibleAnywhere)
  TObjectPtr<USplineComponent> Spline;

  void AutoRun();
};
