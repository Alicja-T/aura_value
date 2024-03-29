// Copyright Philosophical Games


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/ValueAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "ValueGameplayTags.h"

void UAttributeMenuWidgetController::BroadcastInitialValues() {
  UValueAttributeSet* AS = CastChecked<UValueAttributeSet>(AttributeSet);

  check(AttributeInfo);

	for (auto& Pair : AS->TagsToAttributes) {
    FValueAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(Pair.Key);
    Info.AttributeValue = Pair.Value().GetNumericValue(AS);
    AttributeInfoDelegate.Broadcast(Info);
  }
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies() {}
