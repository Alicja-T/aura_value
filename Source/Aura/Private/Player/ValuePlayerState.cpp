// Copyright Philosophical Games

#include "Player/ValuePlayerState.h"
#include "AbilitySystem/ValueAbilitySystemComponent.h"
#include "AbilitySystem/ValueAttributeSet.h"

AValuePlayerState::AValuePlayerState() { 
  NetUpdateFrequency = 100.f;
  AbilitySystemComponent = CreateDefaultSubobject<UValueAbilitySystemComponent>(
      FName("AbilitySystemComponent"));
  AbilitySystemComponent->SetIsReplicated(true);
  AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
  AttributeSet =
      CreateDefaultSubobject<UValueAttributeSet>(FName("AttributeSet"));
}

UAbilitySystemComponent* AValuePlayerState::GetAbilitySystemComponent() const {
  return AbilitySystemComponent;
}
