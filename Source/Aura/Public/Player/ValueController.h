// Copyright Philosophical Games

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "ValueController.generated.h"

struct FInputActionValue;
class IEnemyInterface;
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

  void Move(const FInputActionValue& InputActionValue);
  void CursorTrace();

  IEnemyInterface* LastActor;
  IEnemyInterface* CurrentActor;

  void AbilityInputTagPressed(FGameplayTag InputTag);
  void AbilityInputTagReleased(FGameplayTag InputTag);
  void AbilityInputTagHeld(FGameplayTag AInputTag);


  UPROPERTY(EditDefaultsOnly, Category = Input)
  TObjectPtr<class UValueInputConfig> InputConfig;
};
