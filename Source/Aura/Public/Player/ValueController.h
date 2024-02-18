// Copyright Philosophical Games

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ValueController.generated.h"

struct FInputActionValue;
/**
 * 
 */
UCLASS()
class AURA_API AValueController : public APlayerController
{
	GENERATED_BODY()
 public:
  AValueController();
 protected:
  virtual void BeginPlay() override;
  virtual void SetupInputComponent() override;

 private:
  UPROPERTY(EditAnywhere, Category = Input)
  TObjectPtr<class UInputMappingContext> ValueContext;
  UPROPERTY(EditAnywhere, Category = Input)
  TObjectPtr<class UInputAction> MoveAction;

  void Move(const FInputActionValue& InputActionValue);
};
