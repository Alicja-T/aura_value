// Copyright Philosophical Games


#include "AbilitySystem/ValueAbilitySystemGlobals.h"
#include "ValueAbilityTypes.h"

FGameplayEffectContext* UValueAbilitySystemGlobals::AllocGameplayEffectContext()
    const {
  return new FValueGameplayEffectContext();
}
