// Copyright Philosophical Games


#include "AbilitySystem/ValueAttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameFramework/Character.h"
#include "GameplayEffectExtension.h"
#include <Net/UnrealNetwork.h>

UValueAttributeSet::UValueAttributeSet() {
  InitHealth(50.f);
  InitMaxHealth(100.f);
  InitMana(25.f);
  InitMaxMana(50.f);

}

void UValueAttributeSet::GetLifetimeReplicatedProps(
    TArray<FLifetimeProperty>& OutLifetimeProps) const {
  Super::GetLifetimeReplicatedProps(OutLifetimeProps);

  DOREPLIFETIME_CONDITION_NOTIFY(UValueAttributeSet, Strength, COND_None,
                                 REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UValueAttributeSet, Intelligence, COND_None,
                                 REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UValueAttributeSet, Resilience, COND_None,
                                 REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UValueAttributeSet, Vigor, COND_None,
                                 REPNOTIFY_Always);

  DOREPLIFETIME_CONDITION_NOTIFY(UValueAttributeSet, Health, COND_None,
                                 REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UValueAttributeSet, MaxHealth, COND_None,
                                 REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UValueAttributeSet, Mana, COND_None,
                                 REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UValueAttributeSet, MaxMana, COND_None,
                                 REPNOTIFY_Always);

}

void UValueAttributeSet::SetEffectProperties(
    const FGameplayEffectModCallbackData& Data,
    FEffectProperties& Props) const {
  Props.EffectContextHandle = Data.EffectSpec.GetContext();
  Props.SourceASC =
      Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

  if (IsValid(Props.SourceASC) && Props.SourceASC->AbilityActorInfo.IsValid() &&
      Props.SourceASC->AbilityActorInfo->AvatarActor.IsValid()) {
    Props.SourceAvatarActor = Props.SourceASC->AbilityActorInfo->AvatarActor.Get();
    Props.SourceController = Props.SourceASC->AbilityActorInfo->PlayerController.Get();
    if (Props.SourceController == nullptr &&
        Props.SourceAvatarActor != nullptr) {
      if (const APawn* Pawn = Cast<APawn>(Props.SourceAvatarActor)) {
        Props.SourceController = Pawn->GetController();
      }
    }
    if (Props.SourceController) {
      Props.SourceCharacter =
          Cast<ACharacter>(Props.SourceController->GetPawn());
    }
  }

  if (Data.Target.AbilityActorInfo.IsValid() &&
      Data.Target.AbilityActorInfo->AvatarActor.IsValid()) {
    Props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
    Props.TargetController =
        Data.Target.AbilityActorInfo->PlayerController.Get();
    Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);
    Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(
        Props.TargetAvatarActor);
  }

}

void UValueAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute,
                                            float& NewValue) {
  Super::PreAttributeChange(Attribute, NewValue);
  if (Attribute == GetHealthAttribute()) {
    NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
  }
  if (Attribute == GetManaAttribute()) {
    NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
  }
}

void UValueAttributeSet::PostGameplayEffectExecute(
    const FGameplayEffectModCallbackData& Data) {
  Super::PostGameplayEffectExecute(Data);
  FEffectProperties Props;
  SetEffectProperties(Data, Props);
  if (Data.EvaluatedData.Attribute == GetHealthAttribute()) {
    SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
  }
  if (Data.EvaluatedData.Attribute == GetManaAttribute()) {
    SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));
  }
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


void UValueAttributeSet::OnRep_Strength(
    const FGameplayAttributeData& OldStrength) const {
  GAMEPLAYATTRIBUTE_REPNOTIFY(UValueAttributeSet, Strength, OldStrength);
}

void UValueAttributeSet::OnRep_Intelligence(
    const FGameplayAttributeData& OldIntelligence) const {
  GAMEPLAYATTRIBUTE_REPNOTIFY(UValueAttributeSet, Intelligence, OldIntelligence);
}

void UValueAttributeSet::OnRep_Resilience(
    const FGameplayAttributeData& OldResilience) const {
  GAMEPLAYATTRIBUTE_REPNOTIFY(UValueAttributeSet, Resilience, OldResilience);
}

void UValueAttributeSet::OnRep_Vigor(
    const FGameplayAttributeData& OldVigor) const {
  GAMEPLAYATTRIBUTE_REPNOTIFY(UValueAttributeSet, Vigor, OldVigor);
}

