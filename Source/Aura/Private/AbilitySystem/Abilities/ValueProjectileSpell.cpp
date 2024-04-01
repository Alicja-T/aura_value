// Copyright Philosophical Games


#include "AbilitySystem/Abilities/ValueProjectileSpell.h"
#include "Kismet/KismetSystemLibrary.h"


void UValueProjectileSpell::ActivateAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData) {
  Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

  UKismetSystemLibrary::PrintString(this, FString("ActivateAbility (C++)"),
                                    true, true, FLinearColor::Yellow, 3);


}
