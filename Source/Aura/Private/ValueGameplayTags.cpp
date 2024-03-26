// Copyright Philosophical Games


#include "ValueGameplayTags.h"
#include "GameplayTagsManager.h"

FValueGameplayTags FValueGameplayTags::GameplayTags;

void FValueGameplayTags::InitializeNativeGameplayTags() {
  GameplayTags.Attributes_Secondary_Armor =
      UGameplayTagsManager::Get().AddNativeGameplayTag(
      FName("Attributes.Secondary.Armor"),
      FString("Reduces damage taken, improves Block Chance"));
  GameplayTags.Attributes_Secondary_ArmorPenetration =
      UGameplayTagsManager::Get().AddNativeGameplayTag(
          FName("Attributes.Secondary.ArmorPenetration"),
          FString("Reduces damage taken, improves Block Chance"));
}
