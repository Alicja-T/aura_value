// Copyright Philosophical Games

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ValueGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UValueGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
  UPROPERTY(EditDefaultsOnly, Category = "Input")
  FGameplayTag StartupInputTag;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
  FScalableFloat Damage;
};
