// Copyright Philosophical Games


#include "ValueGameplayTags.h"
#include "GameplayTagsManager.h"

void FValueGameplayTags::InitializeNativeGameplayTags() {
  UGameplayTagsManager::Get().AddNativeGameplayTag(
      FName("Attributes.Secondary.Armor"),
      FString("Reduces damage taken, improves Block Chance"));
}
