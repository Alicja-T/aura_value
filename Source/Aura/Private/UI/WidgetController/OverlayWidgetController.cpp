// Copyright Philosophical Games


#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/ValueAttributeSet.h"
#include "AbilitySystem/ValueAbilitySystemComponent.h"

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
  Cast<UValueAbilitySystemComponent>(AbilitySystemComponent)
      ->EffectAssetTags.AddLambda([this](const FGameplayTagContainer& AssetTags) {
        for (const FGameplayTag& Tag : AssetTags) {
          const FString Msg =
              FString::Printf(TEXT("GE Tag: %s"), *Tag.ToString());
          GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Blue, Msg);
          FUIWidgetRow* Row =
              GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
        }
      });
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
