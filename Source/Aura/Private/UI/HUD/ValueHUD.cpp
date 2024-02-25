// Copyright Philosophical Games


#include "UI/HUD/ValueHUD.h"
#include "UI/Widget/ValueUserWidget.h"
#include "UI/WidgetController/OverlayWidgetController.h"

UOverlayWidgetController* AValueHUD::GetOverlayWidgetController(
    const FWidgetControllerParams& Params) {
  if (OverlayWidgetController == nullptr) {
    OverlayWidgetController =
        NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
    OverlayWidgetController->SetWidgetControllerParams(Params);
    OverlayWidgetController->BindCallbacksToDependencies();
  }
  return OverlayWidgetController;
}

void AValueHUD::InitOverlay(APlayerController* PC, APlayerState* PS,
                            UAbilitySystemComponent* ASC, UAttributeSet* AS) { 

  checkf(OverlayWidgetClass, TEXT("OverlayWidgetClass uninitialized"));
  checkf(OverlayWidgetControllerClass, TEXT("OverlayWidgetControllerClass uninitialized"));
  UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
  OverlayWidget = Cast<UValueUserWidget>(Widget);

  const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
  UOverlayWidgetController* WidgetController =
      GetOverlayWidgetController(WidgetControllerParams);

  OverlayWidget->SetWidgetController(WidgetController);
  WidgetController->BroadcastInitialValues();
  Widget->AddToViewport();

}


