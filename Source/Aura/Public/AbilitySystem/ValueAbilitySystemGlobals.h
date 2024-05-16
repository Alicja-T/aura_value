// Copyright Philosophical Games

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "ValueAbilitySystemGlobals.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UValueAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()
  virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
	
};
