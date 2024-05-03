// Copyright Philosophical Games


#include "AbilitySystem/Abilities/ValueProjectileSpell.h"
#include "Actor/ValueProjectile.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
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
  ICombatInterface* CombatInterface =
      Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
  if (CombatInterface) {
    const FVector SocketLocation = CombatInterface->GetCombatSocketLocation();

    FTransform SpawnTransform;
    FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
    Rotation.Pitch = 0.f;
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
    const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(
        DamageEffectClass, GetAbilityLevel(), SourceASC->MakeEffectContext());
    FValueGameplayTags GameplayTags = FValueGameplayTags::Get();
    const float ScaledDamage = Damage.GetValueAtLevel(20);
    UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
        SpecHandle, GameplayTags.Damage, ScaledDamage);


    Projectile->DamageEffectSpecHandle = SpecHandle;
    Projectile->FinishSpawning(SpawnTransform);
  }
}