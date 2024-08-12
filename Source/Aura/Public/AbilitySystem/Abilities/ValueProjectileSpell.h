// Copyright Philosophical Games

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/ValueDamageAbility.h"
#include "ValueProjectileSpell.generated.h"

class AValueProjectile;
class UGameplayEffect;
struct FGameplayTag;
 /**
 * 
 */
UCLASS()
class AURA_API UValueProjectileSpell : public UValueDamageAbility
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
  void SpawnProjectile(const FVector& ProjectileTargetLocation,
                       const FGameplayTag& SocketTag);

  UPROPERTY(EditAnywhere, BlueprintReadOnly)
  TSubclassOf<AValueProjectile> ProjectileClass;


};

