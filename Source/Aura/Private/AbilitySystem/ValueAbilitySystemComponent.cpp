// Copyright Philosophical Games


#include "AbilitySystem/ValueAbilitySystemComponent.h"
#include "ValueGameplayTags.h"

void UValueAbilitySystemComponent::AbilityActorInfoSet() {
  OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UValueAbilitySystemComponent::EffectApplied);

  	const FValueGameplayTags& GameplayTags = FValueGameplayTags::Get();

}

void UValueAbilitySystemComponent::EffectApplied(
    UAbilitySystemComponent* AbilitySystemComponent,
    const FGameplayEffectSpec& EffectSpec,
    FActiveGameplayEffectHandle ActiveEffectHandle) {
  FGameplayTagContainer TagContainer;
  EffectSpec.GetAllAssetTags(TagContainer);
  EffectAssetTags.Broadcast(TagContainer);
}
