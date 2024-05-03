// Copyright Philosophical Games


#include "AbilitySystem/ValueAttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Interaction/CombatInterface.h"
#include "GameFramework/Character.h"
#include "GameplayEffectExtension.h"
#include <Net/UnrealNetwork.h>
#include "ValueGameplayTags.h"

UValueAttributeSet::UValueAttributeSet() {
  const FValueGameplayTags& GameplayTags = FValueGameplayTags::Get();

  TagsToAttributes.Add(GameplayTags.Attributes_Primary_Strength,
                       GetStrengthAttribute);
  TagsToAttributes.Add(GameplayTags.Attributes_Primary_Intelligence,
                       GetIntelligenceAttribute);
  TagsToAttributes.Add(GameplayTags.Attributes_Primary_Resilience,
                       GetResilienceAttribute);
  TagsToAttributes.Add(GameplayTags.Attributes_Primary_Vigor,
                       GetVigorAttribute);

  TagsToAttributes.Add(GameplayTags.Attributes_Secondary_Armor,
                       GetArmorAttribute);
  TagsToAttributes.Add(GameplayTags.Attributes_Secondary_ArmorPenetration,
                       GetArmorPenetrationAttribute);
  TagsToAttributes.Add(GameplayTags.Attributes_Secondary_BlockChance,
                       GetBlockChanceAttribute);
  TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CriticalHitChance,
                       GetCriticalHitChanceAttribute);
  TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CriticalHitResistance,
                       GetCriticalHitResistanceAttribute);
  TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CriticalHitDamage,
                       GetCriticalHitDamageAttribute);
  TagsToAttributes.Add(GameplayTags.Attributes_Secondary_HealthRegeneration,
                       GetHealthRegenerationAttribute);
  TagsToAttributes.Add(GameplayTags.Attributes_Secondary_ManaRegeneration,
                       GetManaRegenerationAttribute);
  TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MaxHealth,
                       GetMaxHealthAttribute);
  TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MaxMana,
                       GetMaxManaAttribute);
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

  DOREPLIFETIME_CONDITION_NOTIFY(UValueAttributeSet, Mana, COND_None,
                                 REPNOTIFY_Always);

  DOREPLIFETIME_CONDITION_NOTIFY(UValueAttributeSet, Armor, COND_None,
                                 REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UValueAttributeSet, ArmorPenetration, COND_None,
                                 REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UValueAttributeSet, BlockChance, COND_None,
                                 REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UValueAttributeSet, CriticalHitChance, COND_None,
                                 REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UValueAttributeSet, CriticalHitDamage, COND_None,
                                 REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UValueAttributeSet, CriticalHitResistance, COND_None,
                                 REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UValueAttributeSet, HealthRegeneration, COND_None,
                                 REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UValueAttributeSet, ManaRegeneration,
                                 COND_None, REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UValueAttributeSet, MaxHealth, COND_None,
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
    UE_LOG(LogTemp, Warning, TEXT("Changed Health on %s, Health: %f"),
           *Props.TargetAvatarActor->GetName(), GetHealth());
  }
  if (Data.EvaluatedData.Attribute == GetManaAttribute()) {
    SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));
  }
  if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute()) {
    const float LocalIncomingDamage = GetIncomingDamage();
    SetIncomingDamage(0.f);
    if (LocalIncomingDamage > 0.f) {
      const float NewHealth = GetHealth() - LocalIncomingDamage;
      SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));
      bool bFatal = NewHealth <= 0;
      if (bFatal) {
        ICombatInterface* CombatInterface =
            Cast<ICombatInterface>(Props.TargetAvatarActor);
        if (CombatInterface) {
          CombatInterface->Die();
        }
      } 
      else {
        FGameplayTagContainer TagContainer;
        TagContainer.AddTag(FValueGameplayTags::Get().Effects_HitReact);
        Props.TargetASC->TryActivateAbilitiesByTag(TagContainer);
      }
    }
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

void UValueAttributeSet::OnRep_Armor(
    const FGameplayAttributeData& OldArmor) const {
  GAMEPLAYATTRIBUTE_REPNOTIFY(UValueAttributeSet, Armor, OldArmor);
}

void UValueAttributeSet::OnRep_ArmorPenetration(
    const FGameplayAttributeData& OldArmorPenetration) const {
  GAMEPLAYATTRIBUTE_REPNOTIFY(UValueAttributeSet, ArmorPenetration, OldArmorPenetration);
}

void UValueAttributeSet::OnRep_BlockChance(
    const FGameplayAttributeData& OldBlockChance) const {
  GAMEPLAYATTRIBUTE_REPNOTIFY(UValueAttributeSet, BlockChance, OldBlockChance);
}

void UValueAttributeSet::OnRep_CriticalHitChance(
    const FGameplayAttributeData& OldCriticalHitChance) const {
  GAMEPLAYATTRIBUTE_REPNOTIFY(UValueAttributeSet, CriticalHitChance, OldCriticalHitChance);
}

void UValueAttributeSet::OnRep_CriticalHitDamage(
    const FGameplayAttributeData& OldCriticalHitDamage) const {
  GAMEPLAYATTRIBUTE_REPNOTIFY(UValueAttributeSet, CriticalHitDamage, OldCriticalHitDamage);
}

void UValueAttributeSet::OnRep_CriticalHitResistance(
    const FGameplayAttributeData& OldCriticalHitResistance) const {
  GAMEPLAYATTRIBUTE_REPNOTIFY(UValueAttributeSet, CriticalHitResistance, OldCriticalHitResistance);
}

void UValueAttributeSet::OnRep_HealthRegeneration(
    const FGameplayAttributeData& OldHealthRegeneration) const {
  GAMEPLAYATTRIBUTE_REPNOTIFY(UValueAttributeSet, HealthRegeneration, OldHealthRegeneration);
}

void UValueAttributeSet::OnRep_ManaRegeneration(
    const FGameplayAttributeData& OldManaRegeneration) const {
  GAMEPLAYATTRIBUTE_REPNOTIFY(UValueAttributeSet, ManaRegeneration, OldManaRegeneration);
}

