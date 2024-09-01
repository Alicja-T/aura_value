// Copyright Philosophical Games

#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "AbilitySystem/ValueAbilitySystemComponent.h"
#include "AbilitySystem/ValueAttributeSet.h"
#include "Player/ValuePlayerState.h"

void UOverlayWidgetController::BroadcastInitialValues() {
  UValueAttributeSet* ValueAttributeSet =
      CastChecked<UValueAttributeSet>(AttributeSet);
  OnHealthChanged.Broadcast(ValueAttributeSet->GetHealth());
  OnMaxHealthChanged.Broadcast(ValueAttributeSet->GetMaxHealth());
  OnManaChanged.Broadcast(ValueAttributeSet->GetMana());
  OnMaxManaChanged.Broadcast(ValueAttributeSet->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToAttributeSet(
    const FGameplayAttribute& Attribute,
    FOnAttributeChangedSignature* AttributeChangeDelegate) {
  AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attribute)
      .AddLambda([AttributeChangeDelegate](const FOnAttributeChangeData& Data) {
        AttributeChangeDelegate->Broadcast(Data.NewValue);
      });
}

void UOverlayWidgetController::BindCallbacksToDependencies() {
  AValuePlayerState* ValuePlayerState =
      CastChecked<AValuePlayerState>(PlayerState);
  ValuePlayerState->OnXPChangedDelegate.AddUObject(
      this, &UOverlayWidgetController::OnXPChanged);

  UValueAttributeSet* ValueAttributeSet =
      CastChecked<UValueAttributeSet>(AttributeSet);
  BindCallbacksToAttributeSet(ValueAttributeSet->GetHealthAttribute(),
                              &OnHealthChanged);
  BindCallbacksToAttributeSet(ValueAttributeSet->GetMaxHealthAttribute(),
                              &OnMaxHealthChanged);
  BindCallbacksToAttributeSet(ValueAttributeSet->GetManaAttribute(),
                              &OnManaChanged);
  BindCallbacksToAttributeSet(ValueAttributeSet->GetMaxManaAttribute(),
                              &OnMaxManaChanged);

  if (UValueAbilitySystemComponent* ValueASC =
          Cast<UValueAbilitySystemComponent>(AbilitySystemComponent)) {
    if (ValueASC->bStartupAbilitiesGiven) {
      OnInitializeStartupAbilities(ValueASC);
    } else {
      ValueASC->AbilitiesGivenDelegate.AddUObject(
          this, &UOverlayWidgetController::OnInitializeStartupAbilities);
    }
    ValueASC->EffectAssetTags.AddLambda(
        [this](const FGameplayTagContainer& AssetTags) {
          for (const FGameplayTag& Tag : AssetTags) {
            // For example, say that Tag = Message.HealthPotion
            // "Message.HealthPotion".MatchesTag("Message") will return True,
            // "Message".MatchesTag("Message.HealthPotion") will return False
            FGameplayTag MessageTag =
                FGameplayTag::RequestGameplayTag(FName("Message"));
            if (Tag.MatchesTag(MessageTag)) {
              const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(
                  MessageWidgetDataTable, Tag);
              MessageWidgetRowDelegate.Broadcast(*Row);
            }
          }
        });
  }
}

void UOverlayWidgetController::OnInitializeStartupAbilities(
    UValueAbilitySystemComponent* ValueASC) {
  if (!ValueASC->bStartupAbilitiesGiven) return;
  FForEachAbility BroadcastDelegate;
  BroadcastDelegate.BindLambda(
      [this, ValueASC](const FGameplayAbilitySpec& AbilitySpec) {
        // TODO need a way to figure out the ability tag for a given ability
        // spec.
        FValueAbilityInfo Info = AbilityInfo->FindAbilityInfoByTag(
            ValueASC->GetAbilityTagFromSpec(AbilitySpec));
        Info.InputTag = ValueASC->GetInputTagFromSpec(AbilitySpec);
        AbilityInfoDelegate.Broadcast(Info);
      });
  ValueASC->ForEachAbility(BroadcastDelegate);
}

void UOverlayWidgetController::OnXPChanged(int32 NewXP) const {
  AValuePlayerState* ValuePlayerState =
      CastChecked<AValuePlayerState>(PlayerState);
  const ULevelUpInfo* LevelInfo = ValuePlayerState->LevelUpInfo;
  checkf(LevelInfo, TEXT("Unabled to find LevelUpInfo. Please fill out "
                         "ValuePlayerState Blueprint"));
  const int32 Level = LevelInfo->FindLevelForXP(NewXP);
  const int32 MaxLevel = LevelInfo->LevelUpInformation.Num();
  if (Level <= MaxLevel && Level > 0) {
    const int32 LevelUpReq =
        LevelInfo->LevelUpInformation[Level].LevelUpRequirement;
    const int32 PrevLevelReq =
        LevelInfo->LevelUpInformation[Level - 1].LevelUpRequirement;

    const int32 DeltaLevelRequirement = LevelUpReq - PrevLevelReq;
    const int32 XPForThisLevel = NewXP - PrevLevelReq;

    const float XPBarPercent = static_cast<float>(XPForThisLevel) /
                               static_cast<float>(DeltaLevelRequirement);

    OnXPPercentChanged.Broadcast(XPBarPercent);
  }
}
