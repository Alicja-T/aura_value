// Copyright Philosophical Games


#include "AbilitySystem/ValueAbilitySystemLibrary.h"
#include "Game/ValueGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HUD/ValueHUD.h"
#include "Player/ValuePlayerState.h"
#include "UI/WidgetController/ValueWidgetController.h"
#include "ValueAbilityTypes.h"

UOverlayWidgetController*
UValueAbilitySystemLibrary::GetOverlayWidgetController(
    const UObject* WorldContextObject) {
  if (APlayerController* PC =
          UGameplayStatics::GetPlayerController(WorldContextObject, 0)) {
    if (AValueHUD* HUD = Cast<AValueHUD>(PC->GetHUD())) {
      AValuePlayerState* PS = PC->GetPlayerState<AValuePlayerState>(); 
      UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
      UAttributeSet* AS = PS->GetAttributeSet();
      const FWidgetControllerParams WidgetContollerParams(PC, PS, ASC, AS);
      return HUD->GetOverlayWidgetController(WidgetContollerParams);
    }
  }
  return nullptr;
}

UAttributeMenuWidgetController*
UValueAbilitySystemLibrary::GetAttributeMenuWidgetController(
    const UObject* WorldContextObject) {
  if (APlayerController* PC =
          UGameplayStatics::GetPlayerController(WorldContextObject, 0)) {
    if (AValueHUD* HUD = Cast<AValueHUD>(PC->GetHUD())) {
      AValuePlayerState* PS = PC->GetPlayerState<AValuePlayerState>();
      UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
      UAttributeSet* AS = PS->GetAttributeSet();
      const FWidgetControllerParams WidgetContollerParams(PC, PS, ASC, AS);
      return HUD->GetAttributeMenuWidgetController(WidgetContollerParams);
    }
  }
  return nullptr;
}

void UValueAbilitySystemLibrary::InitializeDefaultAttributes(
    const UObject* WorldContextObject, ECharacterClass CharacterClass,
    float Level, UAbilitySystemComponent* ASC) {

  AActor* AvatarActor = ASC->GetAvatarActor();
  UCharacterClassInfo* CharacterInfo = GetCharacterClassInfo(WorldContextObject);
  FCharacterClassDefaultInfo ClassDefaultInfo =
      CharacterInfo->GetClassDefaultInfo(CharacterClass);
  FGameplayEffectContextHandle PrimaryAttributesContextHandle =
      ASC->MakeEffectContext();
  PrimaryAttributesContextHandle.AddSourceObject(AvatarActor);
  const FGameplayEffectSpecHandle PrimaryAttributesSpecHandle =
      ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes, Level,
                            PrimaryAttributesContextHandle);
  ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesSpecHandle.Data.Get());
  
  FGameplayEffectContextHandle SecondaryAttributesContextHandle =
      ASC->MakeEffectContext();
  SecondaryAttributesContextHandle.AddSourceObject(AvatarActor);
  const FGameplayEffectSpecHandle SecondaryAttributesSpecHandle =
      ASC->MakeOutgoingSpec(CharacterInfo->SecondaryAttributes, Level,
                            SecondaryAttributesContextHandle);
  ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data.Get());

  FGameplayEffectContextHandle VitalAttributesContextHandle =
      ASC->MakeEffectContext();
  VitalAttributesContextHandle.AddSourceObject(AvatarActor);
  const FGameplayEffectSpecHandle VitalAttributesSpecHandle =
      ASC->MakeOutgoingSpec(CharacterInfo->VitalAttributes, Level,
                            VitalAttributesContextHandle);
  ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data.Get());
}

void UValueAbilitySystemLibrary::GiveStartupAbilities(
    const UObject* WorldContextObject, UAbilitySystemComponent* ASC) {

  UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
  for (TSubclassOf<UGameplayAbility> AbilityClass :
       CharacterClassInfo->CommonAbilities) {
    FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
    ASC->GiveAbility(AbilitySpec);
  }
}

UCharacterClassInfo* UValueAbilitySystemLibrary::GetCharacterClassInfo(
    const UObject* WorldContextObject) {
  AValueGameModeBase* ValueGameMode = Cast<AValueGameModeBase>(
      UGameplayStatics::GetGameMode(WorldContextObject));
  if (ValueGameMode == nullptr) return nullptr;
  return ValueGameMode->CharacterClassInfo;
}

bool UValueAbilitySystemLibrary::IsBlockedHit(
    const FGameplayEffectContextHandle& EffectContextHandle) {
  if (const FValueGameplayEffectContext* ValueEffectContext =
          static_cast<const FValueGameplayEffectContext*>(
              EffectContextHandle.Get())) {
    return ValueEffectContext->IsBlockedHit();
  }
  return false;
}

bool UValueAbilitySystemLibrary::IsCriticalHit(
    const FGameplayEffectContextHandle& EffectContextHandle) {
  if (const FValueGameplayEffectContext* ValueEffectContext =
          static_cast<const FValueGameplayEffectContext*>(
              EffectContextHandle.Get())) {
    return ValueEffectContext->IsCriticalHit();
  }
  return false;
}

void UValueAbilitySystemLibrary::SetIsBlockedHit(
    UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle,
    bool bInIsBlockedHit) {
  if (FValueGameplayEffectContext* ValueEffectContext =
          static_cast<FValueGameplayEffectContext*>(
              EffectContextHandle.Get())) {
    ValueEffectContext->SetIsBlockedHit(bInIsBlockedHit);
  }

}

void UValueAbilitySystemLibrary::SetIsCriticalHit(
    UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle,
    bool bInIsCriticalHit) {
  if (FValueGameplayEffectContext* ValueEffectContext =
          static_cast<FValueGameplayEffectContext*>(
              EffectContextHandle.Get())) {
    ValueEffectContext->SetIsCriticalHit(bInIsCriticalHit);
  }

}
