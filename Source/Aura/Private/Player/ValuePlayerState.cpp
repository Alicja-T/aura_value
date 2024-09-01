// Copyright Philosophical Games

#include "Player/ValuePlayerState.h"
#include "AbilitySystem/ValueAbilitySystemComponent.h"
#include "AbilitySystem/ValueAttributeSet.h"
#include <Net/UnrealNetwork.h>

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

void AValuePlayerState::GetLifetimeReplicatedProps(
    TArray<FLifetimeProperty>& OutLifetimeProps) const {
  Super::GetLifetimeReplicatedProps(OutLifetimeProps);
  DOREPLIFETIME(AValuePlayerState, Level);
  DOREPLIFETIME(AValuePlayerState, XP);
}

void AValuePlayerState::AddToXP(int32 inXP) { 
  XP += inXP;
  OnXPChangedDelegate.Broadcast(XP);
}

void AValuePlayerState::AddToLevel(int32 InLevel) { 
  Level += InLevel; 
  OnLevelChangedDelegate.Broadcast(Level);
}

void AValuePlayerState::SetXP(int32 inXP) { 
  XP = inXP;
  OnXPChangedDelegate.Broadcast(XP);
}

void AValuePlayerState::SetLevel(int32 InLevel) { 
  Level = InLevel; 
  OnLevelChangedDelegate.Broadcast(Level);
}


void AValuePlayerState::OnRep_Level(int32 OldLevel) {
  OnLevelChangedDelegate.Broadcast(Level);
}

void AValuePlayerState::OnRep_XP(int32 OldXP) {
  OnXPChangedDelegate.Broadcast(XP);
}
