// Copyright Philosophical Games


#include "AbilitySystem/MMC/MMC_MaxMana.h"
#include "AbilitySystem/ValueAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxMana::UMMC_MaxMana() {
  IntelligenceDef.AttributeToCapture = UValueAttributeSet::GetIntelligenceAttribute();
  IntelligenceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
  IntelligenceDef.bSnapshot = false;
  RelevantAttributesToCapture.Add(IntelligenceDef);

}

float UMMC_MaxMana::CalculateBaseMagnitude_Implementation(
    const FGameplayEffectSpec& Spec) const {
  // Gather tags from source and target
  const FGameplayTagContainer* SourceTags =
      Spec.CapturedSourceTags.GetAggregatedTags();
  const FGameplayTagContainer* TargetTags =
      Spec.CapturedTargetTags.GetAggregatedTags();

  FAggregatorEvaluateParameters EvaluationParameters;
  EvaluationParameters.SourceTags = SourceTags;
  EvaluationParameters.TargetTags = TargetTags;

  float Intelligence = 0.f;
  GetCapturedAttributeMagnitude(IntelligenceDef, Spec, EvaluationParameters, Intelligence);
  Intelligence = FMath::Max<float>(Intelligence, 0.f);

  ICombatInterface* CombatInterface =
      Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
  const int32 PlayerLevel = CombatInterface->GetPlayerLevel();

  return 80.f + 5.f * Intelligence + 15.f * PlayerLevel;
}