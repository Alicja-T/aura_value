// Copyright Philosophical Games

#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/ValueAbilitySystemComponent.h"
#include "AbilitySystem/ValueAttributeSet.h"

void UOverlayWidgetController::BroadcastInitialValues() {
  UValueAttributeSet* ValueAttributeSet =
      CastChecked<UValueAttributeSet>(AttributeSet);
  OnHealthChanged.Broadcast(ValueAttributeSet->GetHealth());
  OnMaxHealthChanged.Broadcast(ValueAttributeSet->GetMaxHealth());
  OnManaChanged.Broadcast(ValueAttributeSet->GetMana());
  OnMaxManaChanged.Broadcast(ValueAttributeSet->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToAttributeSet(
    const FGameplayAttribute& Attribute,
    FOnAttributeChangedSignature* AttributeChangeDelegate) {
  AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attribute)
      .AddLambda([AttributeChangeDelegate](const FOnAttributeChangeData& Data) {
        AttributeChangeDelegate->Broadcast(Data.NewValue);
      });
}

void UOverlayWidgetController::BindCallbacksToDependencies() {
  UValueAttributeSet* ValueAttributeSet =
      CastChecked<UValueAttributeSet>(AttributeSet);
  BindCallbacksToAttributeSet(ValueAttributeSet->GetHealthAttribute(),
                              &OnHealthChanged);
  BindCallbacksToAttributeSet(ValueAttributeSet->GetMaxHealthAttribute(),
                              &OnMaxHealthChanged);
  BindCallbacksToAttributeSet(ValueAttributeSet->GetManaAttribute(),
                              &OnManaChanged);
  BindCallbacksToAttributeSet(ValueAttributeSet->GetMaxManaAttribute(),
                              &OnMaxManaChanged);

  if (UValueAbilitySystemComponent* ValueASC =
          Cast<UValueAbilitySystemComponent>(AbilitySystemComponent)) {
    if (ValueASC->bStartupAbilitiesGiven) {
      OnInitializeStartupAbilities(ValueASC);
    } else {
    ValueASC->AbilitiesGivenDelegate.AddUObject(
        this, &UOverlayWidgetController::OnInitializeStartupAbilities);
    
    }
    ValueASC->EffectAssetTags.AddLambda(
        [this](const FGameplayTagContainer& AssetTags) {
          for (const FGameplayTag& Tag : AssetTags) {
            // For example, say that Tag = Message.HealthPotion
            // "Message.HealthPotion".MatchesTag("Message") will return True,
            // "Message".MatchesTag("Message.HealthPotion") will return False
            FGameplayTag MessageTag =
                FGameplayTag::RequestGameplayTag(FName("Message"));
            if (Tag.MatchesTag(MessageTag)) {
              const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(
                  MessageWidgetDataTable, Tag);
              MessageWidgetRowDelegate.Broadcast(*Row);
            }
          }
        });
  }
}


void UOverlayWidgetController::OnInitializeStartupAbilities(
    UValueAbilitySystemComponent* ValueASC) {
  if (!ValueASC->bStartupAbilitiesGiven) return;

}
