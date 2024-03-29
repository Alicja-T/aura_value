// Copyright Philosophical Games


#include "AbilitySystem/ValueAbilitySystemComponent.h"
#include "ValueGameplayTags.h"

void UValueAbilitySystemComponent::AbilityActorInfoSet() {
  OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UValueAbilitySystemComponent::EffectApplied);

  	const FValueGameplayTags& GameplayTags = FValueGameplayTags::Get();

}

void UValueAbilitySystemComponent::AddCharacterAbilities(
    const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities) {

  for (auto AbilityClass : StartupAbilities) {
    FGameplayAbilitySpec Spec = FGameplayAbilitySpec(AbilityClass, 1);
    GiveAbilityAndActivateOnce(Spec);
  }

}

void UValueAbilitySystemComponent::EffectApplied(
    UAbilitySystemComponent* AbilitySystemComponent,
    const FGameplayEffectSpec& EffectSpec,
    FActiveGameplayEffectHandle ActiveEffectHandle) {
  FGameplayTagContainer TagContainer;
  EffectSpec.GetAllAssetTags(TagContainer);
  EffectAssetTags.Broadcast(TagContainer);
}
