// Copyright Philosophical Games


#include "AbilitySystem/Abilities/ValueDamageAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

void UValueDamageAbility::CauseDamage(AActor* TargetActor) {
  FGameplayEffectSpecHandle DamageSpecHandle =
      MakeOutgoingGameplayEffectSpec(DamageEffectClass, 1.f);
  for (TTuple<FGameplayTag, FScalableFloat> Pair : DamageTypes) {
    const float ScaledDamage = Pair.Value.GetValueAtLevel(GetAbilityLevel());
    UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
        DamageSpecHandle, Pair.Key, ScaledDamage);
    UE_LOG(LogTemp, Warning, TEXT("The damage value is: %f"), ScaledDamage);
  }

  GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(
      *DamageSpecHandle.Data.Get(),
      UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));


}
