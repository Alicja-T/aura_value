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
}


