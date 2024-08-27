// Copyright Philosophical Games


#include "AbilitySystem/ValueAbilitySystemComponent.h"
#include "ValueGameplayTags.h"
#include "Aura/ValueLogChannels.h"
#include "AbilitySystem/Abilities/ValueGameplayAbility.h"

void UValueAbilitySystemComponent::AbilityActorInfoSet() {
  OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UValueAbilitySystemComponent::ClientEffectApplied);

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
  bStartupAbilitiesGiven = true;
  AbilitiesGivenDelegate.Broadcast(this);

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

FForEachAbility UValueAbilitySystemComponent::ForEachAbility(
    const FForEachAbility& Delegate) {
  FScopedAbilityListLock ActiveScopeLock(*this);
  for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities()) {
    if (!Delegate.ExecuteIfBound(AbilitySpec)) {
      UE_LOG(LogValue, Error, TEXT("Failed to execute delegate in %hs"),
             __FUNCTION__);
    }
  }

  return FForEachAbility();
}

FGameplayTag UValueAbilitySystemComponent::GetAbilityTagFromSpec(
    const FGameplayAbilitySpec& AbilitySpec) {
  if (AbilitySpec.Ability) {
    for (FGameplayTag Tag : AbilitySpec.Ability.Get()->AbilityTags) {
      if (Tag.MatchesTag(
              FGameplayTag::RequestGameplayTag(FName("Abilities")))) {
        return Tag;
      }
    }
  }
  return FGameplayTag();
}

FGameplayTag UValueAbilitySystemComponent::GetInputTagFromSpec(
    const FGameplayAbilitySpec& AbilitySpec) {
  for (FGameplayTag Tag : AbilitySpec.DynamicAbilityTags) {
    if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("InputTag")))) {
      return Tag;
    }
  }
  return FGameplayTag();
}

void UValueAbilitySystemComponent::ClientEffectApplied_Implementation(
    UAbilitySystemComponent* AbilitySystemComponent,
    const FGameplayEffectSpec& EffectSpec,
    FActiveGameplayEffectHandle ActiveEffectHandle) {
  FGameplayTagContainer TagContainer;
  EffectSpec.GetAllAssetTags(TagContainer);
  EffectAssetTags.Broadcast(TagContainer);
}
