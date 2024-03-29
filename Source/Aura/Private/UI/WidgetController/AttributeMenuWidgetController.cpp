// Copyright Philosophical Games


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/ValueAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "ValueGameplayTags.h"

void UAttributeMenuWidgetController::BroadcastInitialValues() {
  UValueAttributeSet* AS = CastChecked<UValueAttributeSet>(AttributeSet);

  check(AttributeInfo);

	for (auto& Pair : AS->TagsToAttributes) {
    BroadcastAttributeInfo(Pair.Key, Pair.Value());
  }
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies() {
  UValueAttributeSet* AS = CastChecked<UValueAttributeSet>(AttributeSet);
  check(AttributeInfo);
  for (auto& Pair : AS->TagsToAttributes) {
    AbilitySystemComponent
        ->GetGameplayAttributeValueChangeDelegate(Pair.Value())
        .AddLambda([this, Pair](const FOnAttributeChangeData& Data) {
          BroadcastAttributeInfo(Pair.Key, Pair.Value());
        });
  }

}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(
    const FGameplayTag& AttributeTag,
    const FGameplayAttribute& Attribute) const {
  FValueAttributeInfo Info =
      AttributeInfo->FindAttributeInfoForTag(AttributeTag);
  Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
  AttributeInfoDelegate.Broadcast(Info);
}
