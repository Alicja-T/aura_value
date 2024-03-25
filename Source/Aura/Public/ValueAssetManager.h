// Copyright Philosophical Games

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "ValueAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UValueAssetManager : public UAssetManager
{
	GENERATED_BODY()
 public:
        static UValueAssetManager& Get();

 protected:       
        virtual void StartInitialLoading() override;
};
