// Copyright Philosophical Games


#include "AbilitySystem/Abilities/ValueProjectileSpell.h"
#include "Actor/ValueProjectile.h"
#include "Interaction/CombatInterface.h"


void UValueProjectileSpell::ActivateAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData) {
  Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

  const bool isServer = HasAuthority(&ActivationInfo);
  if (!isServer) return;
  ICombatInterface* CombatInterface =
      Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
  if (CombatInterface) {
    const FVector SocketLocation = CombatInterface->GetCombatSocketLocation();

    FTransform SpawnTransform;
    SpawnTransform.SetLocation(SocketLocation);
    // TODO: Set the Projectile Rotation

    AValueProjectile* Projectile =
        GetWorld()->SpawnActorDeferred<AValueProjectile>(
            ProjectileClass, SpawnTransform, GetOwningActorFromActorInfo(),
            Cast<APawn>(GetOwningActorFromActorInfo()),
            ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

    // TODO: Give the Projectile a Gameplay Effect Spec for causing Damage.

    Projectile->FinishSpawning(SpawnTransform);
  }
}
