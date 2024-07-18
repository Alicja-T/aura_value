// Copyright Philosophical Games

#include "AbilitySystem/Abilities/ValueProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Actor/ValueProjectile.h"
#include "Aura/Public/ValueGameplayTags.h"
#include "Interaction/CombatInterface.h"

void UValueProjectileSpell::ActivateAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData) {
  Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UValueProjectileSpell::SpawnProjectile(
    const FVector& ProjectileTargetLocation) {
  const bool isServer = GetAvatarActorFromActorInfo()->HasAuthority();
  if (!isServer) return;
  const FVector SocketLocation =
      ICombatInterface::Execute_GetCombatSocketLocation(
          GetAvatarActorFromActorInfo());

  FTransform SpawnTransform;
  FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
  SpawnTransform.SetLocation(SocketLocation);
  SpawnTransform.SetRotation(Rotation.Quaternion());

  AValueProjectile* Projectile =
      GetWorld()->SpawnActorDeferred<AValueProjectile>(
          ProjectileClass, SpawnTransform, GetOwningActorFromActorInfo(),
          Cast<APawn>(GetOwningActorFromActorInfo()),
          ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
  const UAbilitySystemComponent* SourceASC =
      UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(
          GetAvatarActorFromActorInfo());
  FGameplayEffectContextHandle EffectContextHandle =
      SourceASC->MakeEffectContext();
  EffectContextHandle.SetAbility(this);
  EffectContextHandle.AddSourceObject(Projectile);
  TArray<TWeakObjectPtr<AActor>> Actors;
  Actors.Add(Projectile);
  EffectContextHandle.AddActors(Actors);
  FHitResult HitResult;
  HitResult.Location = ProjectileTargetLocation;
  EffectContextHandle.AddHitResult(HitResult);

  const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(
      DamageEffectClass, GetAbilityLevel(), EffectContextHandle);
  FValueGameplayTags GameplayTags = FValueGameplayTags::Get();
  for (auto& Pair : DamageTypes) {
    const float ScaledDamage = Pair.Value.GetValueAtLevel(GetAbilityLevel());
    UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
        SpecHandle, Pair.Key, ScaledDamage);
  }

  Projectile->DamageEffectSpecHandle = SpecHandle;
  Projectile->FinishSpawning(SpawnTransform);
}