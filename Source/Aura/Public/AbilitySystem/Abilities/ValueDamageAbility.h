// Copyright Philosophical Games

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/ValueGameplayAbility.h"
#include "ValueDamageAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UValueDamageAbility : public UValueGameplayAbility
{
	GENERATED_BODY()
 public:
  UFUNCTION(BlueprintCallable)
  void CauseDamage(AActor* TargetActor);


	protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSubclassOf<UGameplayEffect> DamageEffectClass;

    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    TMap<FGameplayTag, FScalableFloat> DamageTypes;
};
