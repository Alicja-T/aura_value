// Copyright Philosophical Games

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/ValueGameplayAbility.h"
#include "ValueSummonAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UValueSummonAbility : public UValueGameplayAbility
{
	GENERATED_BODY()
  UFUNCTION(BlueprintCallable)
  TArray<FVector> GetSpawnLocations();

  UPROPERTY(EditDefaultsOnly, Category = "Summoning")
  int32 NumMinions = 5;

  UPROPERTY(EditDefaultsOnly, Category = "Summoning")
  TArray<TSubclassOf<APawn>> MinionClasses;

  UPROPERTY(EditDefaultsOnly, Category = "Summoning")
  float MinSpawnDistance = 50.f;

  UPROPERTY(EditDefaultsOnly, Category = "Summoning")
  float MaxSpawnDistance = 250.f;

  UPROPERTY(EditDefaultsOnly, Category = "Summoning")
  float SpawnSpread = 90.f;

  UFUNCTION(BlueprintPure, Category = "Summoning")
  TSubclassOf<APawn> GetRandomMinionClass();
};
