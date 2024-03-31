// Copyright Philosophical Games

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "ValueInputConfig.generated.h"

USTRUCT(BlueprintType)
struct FValueInputAction {
  GENERATED_BODY()

  UPROPERTY(EditDefaultsOnly)
  const class UInputAction* InputAction = nullptr;
  UPROPERTY(EditDefaultsOnly)
  FGameplayTag InputTag = FGameplayTag();
};


/**
 * 
 */
UCLASS()
class AURA_API UValueInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:

  const UInputAction* FindAbilityInputActionForTag(
     const FGameplayTag& InputTag, bool bLogNotFound = false) const;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  TArray<FValueInputAction> AbilityInputActions;
	
};
