// Copyright Philosophical Games


#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/ValueAttributeSet.h"

void UOverlayWidgetController::BroadcastInitialValues() {
  UValueAttributeSet* ValueAttributeSet =
      CastChecked<UValueAttributeSet>(AttributeSet);
  OnHealthChanged.Broadcast(ValueAttributeSet->GetHealth());
  OnMaxHealthChanged.Broadcast(ValueAttributeSet->GetMaxHealth());
  OnManaChanged.Broadcast(ValueAttributeSet->GetMana());
  OnMaxManaChanged.Broadcast(ValueAttributeSet->GetMaxMana());

}

void UOverlayWidgetController::BindCallbacksToDependencies() {
  UValueAttributeSet* ValueAttributeSet =
      CastChecked<UValueAttributeSet>(AttributeSet);
  AbilitySystemComponent
      ->GetGameplayAttributeValueChangeDelegate(
          ValueAttributeSet->GetHealthAttribute())
      .AddUObject(this, &UOverlayWidgetController::HealthChanged);
  AbilitySystemComponent
      ->GetGameplayAttributeValueChangeDelegate(
          ValueAttributeSet->GetMaxHealthAttribute())
      .AddUObject(this, &UOverlayWidgetController::MaxHealthChanged);
  AbilitySystemComponent
      ->GetGameplayAttributeValueChangeDelegate(
          ValueAttributeSet->GetManaAttribute())
      .AddUObject(this, &UOverlayWidgetController::ManaChanged);
  AbilitySystemComponent
      ->GetGameplayAttributeValueChangeDelegate(
          ValueAttributeSet->GetMaxManaAttribute())
      .AddUObject(this, &UOverlayWidgetController::MaxManaChanged);
}

void UOverlayWidgetController::HealthChanged(
    const FOnAttributeChangeData& Data) const {
  OnHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxHealthChanged(
    const FOnAttributeChangeData& Data) const {
  OnMaxHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::ManaChanged(
    const FOnAttributeChangeData& Data) const {
  OnManaChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxManaChanged(
    const FOnAttributeChangeData& Data) const {
  OnMaxManaChanged.Broadcast(Data.NewValue);
}
