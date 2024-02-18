// Copyright Philosophical Games

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ValueController.generated.h"

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

 private:
  UPROPERTY(EditAnywhere, Category = Input)
  TObjectPtr<class UInputMappingContext> ValueContext;
};
