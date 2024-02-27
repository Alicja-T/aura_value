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
  FGameplayEffectSpecHandle SpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffect, 1.f, ContextHandle);
  TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
}





