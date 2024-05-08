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

  ValueDamageStatics() {
    DEFINE_ATTRIBUTE_CAPTUREDEF(UValueAttributeSet, Armor, Target, false);
    DEFINE_ATTRIBUTE_CAPTUREDEF(UValueAttributeSet, BlockChance, Target,
                                false);
    DEFINE_ATTRIBUTE_CAPTUREDEF(UValueAttributeSet, ArmorPenetration, Source, false);
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
  float Damage = Spec.GetSetByCallerMagnitude(FValueGameplayTags::Get().Damage);

  float TargetBlockChance = 0.f;
  ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
      DamageStatics().BlockChanceDef, EvaluationParameters, TargetBlockChance);
  float chance = FMath::RandRange(1, 100);
  bool block = chance < TargetBlockChance;
  Damage = block ? Damage / 2.f : Damage;

  float TargetArmor = 0.f;
  ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
      DamageStatics().ArmorDef, EvaluationParameters, TargetArmor);
  TargetArmor = FMath::Max(TargetArmor, 0.f);

  float SourceArmorPenetration = 0.f;
  ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
      DamageStatics().ArmorPenetrationDef, EvaluationParameters, SourceArmorPenetration);
  SourceArmorPenetration = FMath::Max(SourceArmorPenetration, 0.f);

  UCharacterClassInfo* CCI = UValueAbilitySystemLibrary::GetCharacterClassInfo(this);
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
    
  const FGameplayModifierEvaluatedData EvaluatedData(UValueAttributeSet::GetIncomingDamageAttribute(),
      EGameplayModOp::Additive, Damage);
  OutExecutionOutput.AddOutputModifier(EvaluatedData);

}
