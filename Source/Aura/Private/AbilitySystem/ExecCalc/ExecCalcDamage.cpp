// Copyright Philosophical Games

#include "AbilitySystem/ExecCalc/ExecCalcDamage.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/ValueAttributeSet.h"
#include "ValueGameplayTags.h"

struct ValueDamageStatics {
  DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);

  ValueDamageStatics() {
    DEFINE_ATTRIBUTE_CAPTUREDEF(UValueAttributeSet, Armor, Target, false);
  }


};

static const ValueDamageStatics& DamageStatics() {
  static ValueDamageStatics DStatics;
  return DStatics;
}

UExecCalcDamage::UExecCalcDamage() {
  RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
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

  const FGameplayModifierEvaluatedData EvaluatedData(UValueAttributeSet::GetIncomingDamageAttribute(),
      EGameplayModOp::Additive, Damage);
  OutExecutionOutput.AddOutputModifier(EvaluatedData);

}
