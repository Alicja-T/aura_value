// Copyright Philosophical Games

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/ValueGameplayAbility.h"
#include "ValueProjectileSpell.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UValueProjectileSpell : public UValueGameplayAbility
{
	GENERATED_BODY()
protected:
  virtual void ActivateAbility(
      const FGameplayAbilitySpecHandle Handle,
      const FGameplayAbilityActorInfo* ActorInfo,
      const FGameplayAbilityActivationInfo ActivationInfo,
      const FGameplayEventData* TriggerEventData) override;
};
