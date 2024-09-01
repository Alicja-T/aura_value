// Copyright Philosophical Games


#include "AbilitySystem/ValueAbilitySystemLibrary.h"
#include "Game/ValueGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HUD/ValueHUD.h"
#include "Player/ValuePlayerState.h"
#include "UI/WidgetController/ValueWidgetController.h"
#include "ValueAbilityTypes.h"
#include <Interaction/CombatInterface.h>

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
    const UObject* WorldContextObject, UAbilitySystemComponent* ASC,
    ECharacterClass CharacterClass) {

  UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
  if (CharacterClassInfo == nullptr) return;
  for (TSubclassOf<UGameplayAbility> AbilityClass :
       CharacterClassInfo->CommonAbilities) {
    FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
    ASC->GiveAbility(AbilitySpec);
  }

  const FCharacterClassDefaultInfo& ClassInfo =
      CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
  for (auto ClassAbility : ClassInfo.StartupAbilities) {
    ICombatInterface* CombatInterface =
        Cast<ICombatInterface>(ASC->GetAvatarActor());
    if (CombatInterface) {
      FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(ClassAbility, CombatInterface->GetPlayerLevel());
      ASC->GiveAbility(AbilitySpec);
    }
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

void UValueAbilitySystemLibrary::GetLivePlayersWithinRadius(
    const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors,
    const TArray<AActor*>& ActorsToIgnore, float Radius,
    const FVector& SphereOrigin) {
  FCollisionQueryParams SphereParams;
  SphereParams.AddIgnoredActors(ActorsToIgnore);

  if (const UWorld* World = GEngine->GetWorldFromContextObject(
          WorldContextObject, EGetWorldErrorMode::LogAndReturnNull)) {
    TArray<FOverlapResult> Overlaps;
    World->OverlapMultiByObjectType(
        Overlaps, SphereOrigin, FQuat::Identity,
        FCollisionObjectQueryParams(
            FCollisionObjectQueryParams::InitType::AllDynamicObjects),
        FCollisionShape::MakeSphere(Radius), SphereParams);
    for (FOverlapResult& Overlap : Overlaps) {
      if (Overlap.GetActor()->Implements<UCombatInterface>() &&
          !ICombatInterface::Execute_IsDead(Overlap.GetActor())) {
        OutOverlappingActors.AddUnique(
            ICombatInterface::Execute_GetAvatar(Overlap.GetActor()));
      }
    }
  }

}

bool UValueAbilitySystemLibrary::IsNotFriend(AActor* FirstActor,
                                             AActor* SecondActor) {
  const bool bBothArePlayers = FirstActor->ActorHasTag(FName("Player")) &&
                               SecondActor->ActorHasTag(FName("Player"));
  const bool bBothAreEnemies = FirstActor->ActorHasTag(FName("Enemy")) &&
                               SecondActor->ActorHasTag(FName("Enemy"));
  const bool bFriends = bBothArePlayers || bBothAreEnemies;
  return !bFriends;
}

int32 UValueAbilitySystemLibrary::GetXPForClassAndLevel(
    const UObject* WorldContextObject,
    const ECharacterClass& CharacterClass, const int32 Level) {
  UCharacterClassInfo* ClassInfo =
      GetCharacterClassInfo(WorldContextObject);
  if (ClassInfo == nullptr) return 0;
  FScalableFloat LevelXP =
      ClassInfo->GetClassDefaultInfo(CharacterClass).XPReward;
  return static_cast<int32>(LevelXP.GetValueAtLevel(Level));
}
