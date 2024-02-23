// Copyright Philosophical Games


#include "UI/WidgetController/ValueWidgetController.h"

void UValueWidgetController::SetWidgetControllerParams(
    const FWidgetControllerParams& Params) {
  PlayerController = Params.PlayerController;
  PlayerState = Params.PlayerState;
  AttributeSet = Params.AttributeSet;
  AbilitySystemComponent = Params.AbilitySystemComponent;


}

void UValueWidgetController::BroadcastInitialValues() {}
