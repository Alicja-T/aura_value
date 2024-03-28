// Copyright Philosophical Games


#include "ValueGameplayTags.h"
#include "GameplayTagsManager.h"

FValueGameplayTags FValueGameplayTags::GameplayTags;

void FValueGameplayTags::InitializeNativeGameplayTags() {
  /*
  Primary Attributes 
  */ 
  
  GameplayTags.Attributes_Primary_Strength =
      UGameplayTagsManager::Get().AddNativeGameplayTag(
          FName("Attributes.Primary.Strength"),
          FString("Increases physical damage"));

  GameplayTags.Attributes_Primary_Intelligence =
      UGameplayTagsManager::Get().AddNativeGameplayTag(
          FName("Attributes.Primary.Intelligence"),
          FString("Increases magical damage"));

  GameplayTags.Attributes_Primary_Resilience =
      UGameplayTagsManager::Get().AddNativeGameplayTag(
          FName("Attributes.Primary.Resilience"),
          FString("Increases Armor and Armor Penetration"));

  GameplayTags.Attributes_Primary_Vigor =
      UGameplayTagsManager::Get().AddNativeGameplayTag(
          FName("Attributes.Primary.Vigor"), FString("Increases Health"));


  GameplayTags.Attributes_Secondary_Armor =
      UGameplayTagsManager::Get().AddNativeGameplayTag(
      FName("Attributes.Secondary.Armor"),
      FString("Reduces damage taken, improves Block Chance"));
  GameplayTags.Attributes_Secondary_ArmorPenetration =
      UGameplayTagsManager::Get().AddNativeGameplayTag(
          FName("Attributes.Secondary.ArmorPenetration"),
          FString("Ignores Percentage of enemy Armor, increases Critical Hit Chance"));
  GameplayTags.Attributes_Secondary_BlockChance =
      UGameplayTagsManager::Get().AddNativeGameplayTag(
          FName("Attributes.Secondary.BlockChance"),
          FString("Determines the chance to block an attack"));
  GameplayTags.Attributes_Secondary_CriticalHitChance =
      UGameplayTagsManager::Get().AddNativeGameplayTag(
          FName("Attributes.Secondary.CriticalHitChance"),
          FString("Determines the chance of doubling the damage"));
  GameplayTags.Attributes_Secondary_CriticalHitDamage =
      UGameplayTagsManager::Get().AddNativeGameplayTag(
          FName("Attributes.Secondary.CriticalHitDamage"),
          FString("Determines the damage dealt on Critical Hit"));
  GameplayTags.Attributes_Secondary_CriticalHitResistance =
      UGameplayTagsManager::Get().AddNativeGameplayTag(
          FName("Attributes.Secondary.CriticalHitResistance"),
          FString("Reduces Critical Hit Chance of attacking enemies"));
  GameplayTags.Attributes_Secondary_HealthRegeneration =
      UGameplayTagsManager::Get().AddNativeGameplayTag(
          FName("Attributes.Secondary.HealthRegeneration"),
          FString("Sets the rate of health regeneration per second"));
  GameplayTags.Attributes_Secondary_ManaRegeneration =
      UGameplayTagsManager::Get().AddNativeGameplayTag(
          FName("Attributes.Secondary.ManaRegeneration"),
          FString("Sets the rate of mana regenration per second"));
  GameplayTags.Attributes_Secondary_MaxHealth =
      UGameplayTagsManager::Get().AddNativeGameplayTag(
          FName("Attributes.Secondary.MaxHealth"),
          FString("Maximum Health points"));
  GameplayTags.Attributes_Secondary_MaxMana =
      UGameplayTagsManager::Get().AddNativeGameplayTag(
          FName("Attributes.Secondary.MaxMana"),
          FString("Maximum Mana Points"));
}
