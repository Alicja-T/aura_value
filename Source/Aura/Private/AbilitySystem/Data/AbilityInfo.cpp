// Copyright Philosophical Games


#include "AbilitySystem/Data/AbilityInfo.h"
#include "Aura/ValueLogChannels.h"

FValueAbilityInfo UAbilityInfo::FindAbilityInfoByTag(
    const FGameplayTag& AbilityTag, bool bLogNotFound) const {

  for (const FValueAbilityInfo& AbilityInfo : AbilityInformation) {
    if (AbilityInfo.AbilityTag.MatchesTag(AbilityTag)) {
      return AbilityInfo;
    }
  }
  if (bLogNotFound) {
    UE_LOG(LogValue, Error,
           TEXT("Can't find info for AbilityTag [%s] on AbilityInfo [%s]"),
           *AbilityTag.ToString(), *GetNameSafe(this));
  }
  return FValueAbilityInfo();
}
