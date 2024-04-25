// Copyright Philosophical Games


#include "AbilitySystem/ValueAbilitySystemLibrary.h"
#include "Game/ValueGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HUD/ValueHUD.h"
#include "Player/ValuePlayerState.h"
#include "UI/WidgetController/ValueWidgetController.h"

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
  AValueGameModeBase* GameMode = Cast<AValueGameModeBase>(
      UGameplayStatics::GetGameMode(WorldContextObject));
  if (GameMode == nullptr) return;
  AActor* AvatarActor = ASC->GetAvatarActor();
  UCharacterClassInfo* CharacterInfo = GameMode->CharacterClassInfo;
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
