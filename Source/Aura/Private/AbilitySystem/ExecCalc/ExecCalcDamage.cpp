// Copyright Philosophical Games

#include "AbilitySystem/ExecCalc/ExecCalcDamage.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/ValueAttributeSet.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "AbilitySystem/ValueAbilitySystemLibrary.h"
#include "ValueGameplayTags.h"
#include "Interaction/CombatInterface.h"

struct ValueDamageStatics {
  DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);

  DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
  
  DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);

  DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);

  DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance);

  DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);

  ValueDamageStatics() {
    DEFINE_ATTRIBUTE_CAPTUREDEF(UValueAttributeSet, Armor, Target, false);
    DEFINE_ATTRIBUTE_CAPTUREDEF(UValueAttributeSet, BlockChance, Target,
                                false);
    DEFINE_ATTRIBUTE_CAPTUREDEF(UValueAttributeSet, ArmorPenetration, Source, false);
    DEFINE_ATTRIBUTE_CAPTUREDEF(UValueAttributeSet, CriticalHitChance, Source,
                                false);
    DEFINE_ATTRIBUTE_CAPTUREDEF(UValueAttributeSet, CriticalHitResistance, Target,
                                false);
    DEFINE_ATTRIBUTE_CAPTUREDEF(UValueAttributeSet, CriticalHitDamage, Source,
                                false);
  }


};

static const ValueDamageStatics& DamageStatics() {
  static ValueDamageStatics DStatics;
  return DStatics;
}

UExecCalcDamage::UExecCalcDamage() {
  RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
  RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef);
  RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
  RelevantAttributesToCapture.Add(DamageStatics().CriticalHitChanceDef);
  RelevantAttributesToCapture.Add(DamageStatics().CriticalHitResistanceDef);
  RelevantAttributesToCapture.Add(DamageStatics().CriticalHitDamageDef);
}

void UExecCalcDamage::Execute_Implementation(
    const FGameplayEffectCustomExecutionParameters& ExecutionParams,
    FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const {
  const UAbilitySystemComponent* SourceASC =
      ExecutionParams.GetSourceAbilitySystemComponent();
  const UAbilitySystemComponent* TargetASC =
      ExecutionParams.GetTargetAbilitySystemComponent();
  AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
  AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;
  ICombatInterface* SourceCombatInterface = Cast<ICombatInterface>(SourceAvatar);
  ICombatInterface* TargetCombatInterface = Cast<ICombatInterface>(TargetAvatar);
  const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
  const FGameplayTagContainer* SourceTags =
      Spec.CapturedSourceTags.GetAggregatedTags();
  const FGameplayTagContainer* TargetTags =
      Spec.CapturedTargetTags.GetAggregatedTags();
  FAggregatorEvaluateParameters EvaluationParameters;
  EvaluationParameters.SourceTags = SourceTags;
  EvaluationParameters.TargetTags = TargetTags;

  // Get Damage Set by Caller Magnitude
  float Damage = 0.f;
  for (const TTuple<FGameplayTag, FGameplayTag>& Pair :
       FValueGameplayTags::Get().DamageTypesToResistances) {
    const float DamageTypeValue = Spec.GetSetByCallerMagnitude(Pair.Key);
    Damage += DamageTypeValue;
  }
 
  float TargetBlockChance = 0.f;
  ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
      DamageStatics().BlockChanceDef, EvaluationParameters, TargetBlockChance);
  float chance = FMath::RandRange(1, 100);
  bool bBlocked = chance < TargetBlockChance;
  Damage = bBlocked ? Damage / 2.f : Damage;
  FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();

  UValueAbilitySystemLibrary::SetIsBlockedHit(EffectContextHandle, bBlocked);

  float TargetArmor = 0.f;
  ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
      DamageStatics().ArmorDef, EvaluationParameters, TargetArmor);
  TargetArmor = FMath::Max(TargetArmor, 0.f);

  float SourceArmorPenetration = 0.f;
  ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
      DamageStatics().ArmorPenetrationDef, EvaluationParameters, SourceArmorPenetration);
  SourceArmorPenetration = FMath::Max(SourceArmorPenetration, 0.f);

  //TODO rethink coefficients, make them dependent on the difference in levels.

  UCharacterClassInfo* CCI =
      UValueAbilitySystemLibrary::GetCharacterClassInfo(SourceAvatar);
  FRealCurve* ArmorPenetrationCurve = CCI->DamageCalcCoefficients->FindCurve(
      FName("ArmorPenetration"), FString());
  const float ArmorPenetrationCoefficient =
      ArmorPenetrationCurve->Eval(SourceCombatInterface->GetPlayerLevel());
  FRealCurve* EffectiveArmorCurve = CCI->DamageCalcCoefficients->FindCurve(
      FName("EffectiveArmor"), FString());
  const float EffectiveArmorCoefficient =
      EffectiveArmorCurve->Eval(TargetCombatInterface->GetPlayerLevel());

  // ArmorPenetration ignores a percentage of the Target's Armor.
  const float EffectiveArmor = TargetArmor * (100 - SourceArmorPenetration * ArmorPenetrationCoefficient) / 100.f;
  // Armor ignores a percentage of incoming Damage.
  Damage *= (100 - EffectiveArmor * 0.333f) / 100.f;

   
  float SourceCriticalHitChance = 0.f;
  ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
      DamageStatics().CriticalHitChanceDef, EvaluationParameters,
      SourceCriticalHitChance);
  float Chance = FMath::RandRange(1, 100);
  bool bCriticalHit = Chance < SourceCriticalHitChance;

  float TargetCriticalHitResistance = 0.f;
  ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
      DamageStatics().CriticalHitResistanceDef, EvaluationParameters,
      TargetCriticalHitResistance);
  float CriticalHitDamage = 0.f;
  ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
      DamageStatics().CriticalHitDamageDef, EvaluationParameters,
      CriticalHitDamage);
  Chance = FMath::RandRange(1, 100);
  bCriticalHit = Chance < TargetCriticalHitResistance;
  Damage = bCriticalHit ? (Damage * 2.f) + CriticalHitDamage : Damage;
  UValueAbilitySystemLibrary::SetIsCriticalHit(EffectContextHandle,
                                               bCriticalHit);
    
  const FGameplayModifierEvaluatedData EvaluatedData(UValueAttributeSet::GetIncomingDamageAttribute(),
      EGameplayModOp::Additive, Damage);
  OutExecutionOutput.AddOutputModifier(EvaluatedData);

}
