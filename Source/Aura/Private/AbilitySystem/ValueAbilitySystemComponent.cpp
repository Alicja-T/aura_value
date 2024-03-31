// Copyright Philosophical Games


#include "AbilitySystem/ValueAbilitySystemComponent.h"
#include "ValueGameplayTags.h"
#include "AbilitySystem/Abilities/ValueGameplayAbility.h"

void UValueAbilitySystemComponent::AbilityActorInfoSet() {
  OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UValueAbilitySystemComponent::EffectApplied);

  	const FValueGameplayTags& GameplayTags = FValueGameplayTags::Get();

}

void UValueAbilitySystemComponent::AddCharacterAbilities(
    const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities) {

  	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities) {
    FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
    if (const UValueGameplayAbility* AuraAbility =
            Cast<UValueGameplayAbility>(AbilitySpec.Ability)) {
      AbilitySpec.DynamicAbilityTags.AddTag(AuraAbility->StartupInputTag);
      GiveAbility(AbilitySpec);
    }
  }

}

void UValueAbilitySystemComponent::AbilityInputTagHeld(
    const FGameplayTag& InputTag) {
  if (!InputTag.IsValid()) return;

  for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities()) {
    if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)) {
      AbilitySpecInputPressed(AbilitySpec);
      if (!AbilitySpec.IsActive()) {
        TryActivateAbility(AbilitySpec.Handle);
      }
    }
  }
}

void UValueAbilitySystemComponent::AbilityInputTagReleased(
    const FGameplayTag& InputTag) {
  if (!InputTag.IsValid()) return;

  for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities()) {
    if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)) {
      AbilitySpecInputReleased(AbilitySpec);
    }
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
