// Copyright Philosophical Games

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "AbilityInfo.generated.h"

USTRUCT(BlueprintType)
struct FValueAbilityInfo {

	GENERATED_BODY()

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AbilityTag = FGameplayTag();

	UPROPERTY(BlueprintReadOnly)
  FGameplayTag InputTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  FGameplayTag CooldownTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  TObjectPtr<const UTexture2D> IconTexture = nullptr;
  
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)      
  TObjectPtr<const UMaterialInstance> Background = nullptr;   


};

/**
 * 
 */
UCLASS()
class AURA_API UAbilityInfo : public UDataAsset
{
	GENERATED_BODY()

	public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AbilityInformation") 
		TArray<FValueAbilityInfo> AbilityInformation;

		FValueAbilityInfo FindAbilityInfoByTag(
                    const FGameplayTag& AbilityTag, bool bLogNotFound = false) const;
	
};