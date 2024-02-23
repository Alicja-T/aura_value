// Copyright Philosophical Games


#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/ValueAttributeSet.h"

void UOverlayWidgetController::BroadcastInitialValues() {
  UValueAttributeSet* ValueAttributeSet =
      CastChecked<UValueAttributeSet>(AttributeSet);
  OnHealthChanged.Broadcast(ValueAttributeSet->GetHealth());
  OnMaxHealthChanged.Broadcast(ValueAttributeSet->GetMaxHealth());

}
