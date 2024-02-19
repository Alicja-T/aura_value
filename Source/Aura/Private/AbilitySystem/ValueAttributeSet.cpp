// Copyright Philosophical Games


#include "AbilitySystem/ValueAttributeSet.h"

#include <Net/UnrealNetwork.h>

UValueAttributeSet::UValueAttributeSet() {
  InitHealth(100.f);
  InitMaxHealth(100.f);
  InitMana(50.f);
  InitMaxMana(50.f);

}

void UValueAttributeSet::GetLifetimeReplicatedProps(
    TArray<FLifetimeProperty>& OutLifetimeProps) const {
  Super::GetLifetimeReplicatedProps(OutLifetimeProps);
  DOREPLIFETIME_CONDITION_NOTIFY(UValueAttributeSet, Health, COND_None,
                                 REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UValueAttributeSet, MaxHealth, COND_None,
                                 REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UValueAttributeSet, Mana, COND_None,
                                 REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UValueAttributeSet, MaxMana, COND_None,
                                 REPNOTIFY_Always);

}

void UValueAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const {
  GAMEPLAYATTRIBUTE_REPNOTIFY(UValueAttributeSet, Health, OldHealth);


}

void UValueAttributeSet::OnRep_MaxHealth(
    const FGameplayAttributeData& OldMaxHealth) const {
  GAMEPLAYATTRIBUTE_REPNOTIFY(UValueAttributeSet, MaxHealth, OldMaxHealth);

}

void UValueAttributeSet::OnRep_Mana(
    const FGameplayAttributeData& OldMana) const {
  GAMEPLAYATTRIBUTE_REPNOTIFY(UValueAttributeSet, Mana, OldMana);
}

void UValueAttributeSet::OnRep_MaxMana(
    const FGameplayAttributeData& OldMaxMana) const {
  GAMEPLAYATTRIBUTE_REPNOTIFY(UValueAttributeSet, MaxMana, OldMaxMana);
}
