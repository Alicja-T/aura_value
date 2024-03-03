// Copyright Philosophical Games


#include "Actor/ValueEffectActor.h"
#include <AbilitySystem/ValueAbilitySystemComponent.h>
#include <AbilitySystemBlueprintLibrary.h>

// Sets default values
AValueEffectActor::AValueEffectActor() {
  PrimaryActorTick.bCanEverTick = false;
  SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot")));
}


void AValueEffectActor::BeginPlay() {
	Super::BeginPlay();

}

void AValueEffectActor::ApplyEffectToTarget(
    AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffect) {
  UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
  if (TargetASC == nullptr) return;
  check(GameplayEffect);
  FGameplayEffectContextHandle ContextHandle = TargetASC->MakeEffectContext();
  ContextHandle.AddSourceObject(this);
  FGameplayEffectSpecHandle SpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffect, ActorLevel, ContextHandle);
  FActiveGameplayEffectHandle EffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
  bool bIsInfite = SpecHandle.Data->Def.Get()->DurationPolicy ==
                   EGameplayEffectDurationType::Infinite;
  if (bIsInfite) {
    EffectHandleMap.Add(EffectHandle, TargetASC);
  }
}

void AValueEffectActor::OnOverlap(AActor* TargetActor) {
  if (InstantEffectApplicationPolicy ==
      EEffectApplicationPolicy::ApplyOnOverlap) {
    ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
  }
  if (DurationEffectApplicationPolicy ==
      EEffectApplicationPolicy::ApplyOnOverlap) {
    ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
  }
  if (InfiniteEffectApplicationPolicy ==
      EEffectApplicationPolicy::ApplyOnOverlap) {
    ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
  }
}

void AValueEffectActor::OnEndOverlap(AActor* TargetActor) {
  if (InstantEffectApplicationPolicy ==
      EEffectApplicationPolicy::ApplyOnEndOverlap) {
    ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
  }
  if (DurationEffectApplicationPolicy ==
      EEffectApplicationPolicy::ApplyOnEndOverlap) {
    ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
  }
  if (InfiniteEffectApplicationPolicy ==
      EEffectApplicationPolicy::ApplyOnEndOverlap) {  
    ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
  }
  if (InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap) {
    UAbilitySystemComponent* TargetASC =
        UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

    TArray<FActiveGameplayEffectHandle> EffectsToRemove;
    for (auto EffectPair : EffectHandleMap) {
      if (TargetASC == EffectPair.Value) {
        TargetASC->RemoveActiveGameplayEffect(EffectPair.Key, 1);
        EffectsToRemove.Add(EffectPair.Key);
      }
    } 
    for (auto EffectHandle : EffectsToRemove) {
      EffectHandleMap.FindAndRemoveChecked(EffectHandle);
    }
  }
}





