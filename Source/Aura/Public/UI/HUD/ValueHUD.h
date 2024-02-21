// Copyright Philosophical Games

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ValueHUD.generated.h"

class UValueUserWidget;

/**
 * 
 */
UCLASS()
class AURA_API AValueHUD : public AHUD
{
	GENERATED_BODY()
 public:
  UPROPERTY()
  TObjectPtr<UValueUserWidget> OverlayWidget;

 protected:
  virtual void BeginPlay() override;

 private:
  UPROPERTY(EditAnywhere)
  TSubclassOf<UValueUserWidget> OverlayWidgetClass;
	
};
