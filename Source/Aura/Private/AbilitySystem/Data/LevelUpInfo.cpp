// Copyright Philosophical Games

#include "AbilitySystem/Data/LevelUpInfo.h"

int32 ULevelUpInfo::FindLevelForXP(const int32 XP) const {
  for (int i = 1; i < LevelUpInformation.Num(); i++) {
    if (XP < LevelUpInformation[i].LevelUpRequirement) {
      return i;
    }
  }
  return 1;
}
