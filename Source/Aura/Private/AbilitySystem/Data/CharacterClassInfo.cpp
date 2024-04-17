// Copyright Philosophical Games


#include "AbilitySystem/Data/CharacterClassInfo.h"

FCharacterClassDefaultInfo UCharacterClassInfo::GetClassDefaultInfo(
    ECharacterClass CharacterClass) {
  return CharacterClassInfoMap.FindChecked(CharacterClass);
}
