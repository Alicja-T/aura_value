// Copyright Philosophical Games


#include "ValueAssetManager.h"
#include "ValueGameplayTags.h"

UValueAssetManager& UValueAssetManager::Get() {
  check(GEngine);

  UValueAssetManager* ValueAssetManager =
      Cast<UValueAssetManager>(GEngine->AssetManager);
  return *ValueAssetManager;
}

void UValueAssetManager::StartInitialLoading() {
  Super::StartInitialLoading();

  FValueGameplayTags::InitializeNativeGameplayTags();
}
