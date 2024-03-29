// Copyright Philosophical Games


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/ValueAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "ValueGameplayTags.h"

void UAttributeMenuWidgetController::BroadcastInitialValues() {
  UValueAttributeSet* AS = CastChecked<UValueAttributeSet>(AttributeSet);

  check(AttributeInfo);

  FValueAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(
      FValueGameplayTags::Get().Attributes_Primary_Strength);
  Info.AttributeValue = AS->GetStrength();
  AttributeInfoDelegate.Broadcast(Info);
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies() {}
