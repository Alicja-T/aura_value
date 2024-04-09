// Copyright Philosophical Games


#include "ValueAssetManager.h"
#include "ValueGameplayTags.h"
#include "AbilitySystemGlobals.h"

UValueAssetManager& UValueAssetManager::Get() {
  check(GEngine);

  UValueAssetManager* ValueAssetManager =
      Cast<UValueAssetManager>(GEngine->AssetManager);
  return *ValueAssetManager;
}

void UValueAssetManager::StartInitialLoading() {
  Super::StartInitialLoading();

  FValueGameplayTags::InitializeNativeGameplayTags();

  // This is required to use Target Data!
  UAbilitySystemGlobals::Get().InitGlobalData();
}
