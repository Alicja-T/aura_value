// Copyright Philosophical Games

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 *  ValueGameplayTags singleton
 */
struct FValueGameplayTags {
 public:
  static const FValueGameplayTags& Get() { return GameplayTags; }
  static void InitializeNativeGameplayTags();
  FGameplayTag Attributes_Secondary_Armor;
  FGameplayTag Attributes_Secondary_ArmorPenetration;
  FGameplayTag Attributes_Secondary_BlockChance;
  FGameplayTag Attributes_Secondary_CriticalHitChance;

 protected:


 private:
  static FValueGameplayTags GameplayTags;

};