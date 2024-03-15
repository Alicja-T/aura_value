// Copyright Philosophical Games


#include "Character/ValueCharacterBase.h"
#include "AbilitySystem/ValueAbilitySystemComponent.h"
#include "AbilitySystem/ValueAttributeSet.h"
#include "AbilitySystemComponent.h"

AValueCharacterBase::AValueCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

UAbilitySystemComponent* AValueCharacterBase::GetAbilitySystemComponent()
    const {
        return AbilitySystemComponent;
}

// Called when the game starts or when spawned
void AValueCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AValueCharacterBase::InitAbilityActorInfo() {}

void AValueCharacterBase::InitializePrimaryAttributes() const {
  check(IsValid(GetAbilitySystemComponent()));
  check(DefaultPrimaryAttributes);
  const FGameplayEffectContextHandle ContextHandle =
      GetAbilitySystemComponent()->MakeEffectContext();
  const FGameplayEffectSpecHandle SpecHandle =
      GetAbilitySystemComponent()->MakeOutgoingSpec(DefaultPrimaryAttributes,
                                                    1.f, ContextHandle);
  GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(
      *SpecHandle.Data.Get(), GetAbilitySystemComponent());
}



