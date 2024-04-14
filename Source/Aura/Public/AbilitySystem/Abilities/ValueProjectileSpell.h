// Copyright Philosophical Games

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/ValueGameplayAbility.h"
#include "ValueProjectileSpell.generated.h"

class AValueProjectile;
class UGameplayEffect;
    /**
 * 
 */
UCLASS()
class AURA_API UValueProjectileSpell : public UValueGameplayAbility
{
	GENERATED_BODY()

public:


protected:
  virtual void ActivateAbility(
      const FGameplayAbilitySpecHandle Handle,
      const FGameplayAbilityActorInfo* ActorInfo,
      const FGameplayAbilityActivationInfo ActivationInfo,
      const FGameplayEventData* TriggerEventData) override;

  UFUNCTION(BlueprintCallable)
  void SpawnProjectile(const FVector& ProjectileTargetLocation);

  UPROPERTY(EditAnywhere, BlueprintReadOnly)
  TSubclassOf<AValueProjectile> ProjectileClass;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  TSubclassOf<UGameplayEffect> DamageEffectClass;
};

