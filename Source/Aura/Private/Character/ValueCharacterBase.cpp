// Copyright Philosophical Games


#include "Character/ValueCharacterBase.h"
#include "Aura/Aura.h"
#include "AbilitySystem/ValueAbilitySystemComponent.h"
#include "AbilitySystem/ValueAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "ValueGameplayTags.h"
#include "Components/CapsuleComponent.h"

AValueCharacterBase::AValueCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;
  GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
  GetCapsuleComponent()->SetGenerateOverlapEvents(false);
  GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
  GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
  GetMesh()->SetGenerateOverlapEvents(true);
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

UAbilitySystemComponent* AValueCharacterBase::GetAbilitySystemComponent()
    const {
        return AbilitySystemComponent;
}

UAnimMontage* AValueCharacterBase::GetHitReactMontage_Implementation() {
  return HitReactMontage;
}

void AValueCharacterBase::Die() { 
  Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
  MulticastHandleDeath(); 
}

UNiagaraSystem* AValueCharacterBase::GetBloodEffect_Implementation() {
  return BloodEffect;
}

void AValueCharacterBase::MulticastHandleDeath_Implementation() {
  Weapon->SetSimulatePhysics(true);
  Weapon->SetEnableGravity(true);
  Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

  GetMesh()->SetSimulatePhysics(true);
  GetMesh()->SetEnableGravity(true);
  GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
  GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic,
                                           ECollisionResponse::ECR_Block);
  GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
  Dissolve();
  bDead = true;
}

// Called when the game starts or when spawned
void AValueCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

FVector AValueCharacterBase::GetCombatSocketLocation_Implementation(
    const FGameplayTag& MontageTag) { 
  const FValueGameplayTags& GameplayTags = FValueGameplayTags::Get();
  if (MontageTag.MatchesTagExact(GameplayTags.Montage_Attack_Weapon) && IsValid(Weapon)) {
    return Weapon->GetSocketLocation(WeaponTipSocketName);
  } else if (MontageTag.MatchesTagExact(GameplayTags.Montage_Attack_LeftHand)) {
    return GetMesh()->GetSocketLocation(LeftHandSocketName);
  } else if (MontageTag.MatchesTagExact(
                 GameplayTags.Montage_Attack_RightHand)) {
    return GetMesh()->GetSocketLocation(RightHandSocketName);
  }
  return FVector();
}

bool AValueCharacterBase::IsDead_Implementation() const { 
  return bDead; 
}

AActor* AValueCharacterBase::GetAvatar_Implementation() {
  return this; }

TArray<FTaggedMontage> AValueCharacterBase::GetAttackMontages_Implementation() {
  return AttackMontages;
}

void AValueCharacterBase::InitAbilityActorInfo() {}

void AValueCharacterBase::ApplyEffectToSelf(
    TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const {
  check(IsValid(GetAbilitySystemComponent()));
  check(GameplayEffectClass);
  FGameplayEffectContextHandle ContextHandle =
      GetAbilitySystemComponent()->MakeEffectContext();
  ContextHandle.AddSourceObject(this);
  const FGameplayEffectSpecHandle SpecHandle =
      GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, Level,
                                                    ContextHandle);
  GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(
      *SpecHandle.Data.Get(), GetAbilitySystemComponent());
}

void AValueCharacterBase::InitializeDefaultAttributes() const {
  ApplyEffectToSelf(DefaultPrimaryAttributes, 1.f);
  ApplyEffectToSelf(DefaultSecondaryAttributes, 1.f);
  ApplyEffectToSelf(DefaultVitalAttributes, 1.f);
}

void AValueCharacterBase::AddCharacterAbilities() { 
  if (!HasAuthority()) return;
  UValueAbilitySystemComponent* ASC =
      CastChecked<UValueAbilitySystemComponent>(AbilitySystemComponent);
  ASC->AddCharacterAbilities(StartupAbilities);
}

void AValueCharacterBase::Dissolve() { 
  if (IsValid(DissolveMaterialInstance)) {
    UMaterialInstanceDynamic* DynamicMatInst =
        UMaterialInstanceDynamic::Create(DissolveMaterialInstance, this);
    GetMesh()->SetMaterial(0, DynamicMatInst);
    StartDissolveTimeline(DynamicMatInst);
  }
  if (IsValid(WeaponDissolveMaterialInstance)) {
    UMaterialInstanceDynamic* DynamicMatInst =
        UMaterialInstanceDynamic::Create(WeaponDissolveMaterialInstance, this);
    Weapon->SetMaterial(0, DynamicMatInst);
    StartWeaponDissolveTimeline(DynamicMatInst);
  }
}


