// Copyright Philosophical Games

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "ValueAbilitySystemComponent.generated.h"


DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags,
                                    const FGameplayTagContainer& /*AssetTags*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FAbilitiesGiven,
                                    UValueAbilitySystemComponent* ValueASC);
/**
 * 
 */
UCLASS()
class AURA_API UValueAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
 public:
  void AbilityActorInfoSet();
  FEffectAssetTags EffectAssetTags;
  void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);
  void AbilityInputTagHeld(const FGameplayTag& InputTag);
  void AbilityInputTagReleased(const FGameplayTag& InputTag);
  FAbilitiesGiven AbilitiesGivenDelegate;
  bool bStartupAbilitiesGiven = false;
 protected:
	UFUNCTION(Client, Reliable)
	void ClientEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);
	
};
