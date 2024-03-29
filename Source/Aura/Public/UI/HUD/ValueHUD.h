// Copyright Philosophical Games

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ValueHUD.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;
class UValueUserWidget;
class UOverlayWidgetController;
struct FWidgetControllerParams;
class UAttributeMenuWidgetController;

/**
 * 
 */
UCLASS()
class AURA_API AValueHUD : public AHUD
{
	GENERATED_BODY()
 public:

  UOverlayWidgetController* GetOverlayWidgetController(
      const FWidgetControllerParams& Params);
  void InitOverlay(APlayerController* PC, APlayerState* PS,
                   UAbilitySystemComponent* ASC, UAttributeSet* AS);

  UAttributeMenuWidgetController* GetAttributeMenuWidgetController(
      const FWidgetControllerParams& Params);
  void InitAttributeMenu(APlayerController* PC, APlayerState* PS,
                   UAbilitySystemComponent* ASC, UAttributeSet* AS);

 private:
  UPROPERTY()
  TObjectPtr<UValueUserWidget> OverlayWidget;
  UPROPERTY(EditAnywhere)
  TSubclassOf<UValueUserWidget> OverlayWidgetClass;
  UPROPERTY()
  TObjectPtr<UOverlayWidgetController> OverlayWidgetController;
  UPROPERTY(EditAnywhere)
  TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;
  UPROPERTY()
  TObjectPtr<UAttributeMenuWidgetController> AttributeMenuWidgetController;
  UPROPERTY(EditAnywhere)
  TSubclassOf<UAttributeMenuWidgetController> AttributeMenuControllerClass;
};
