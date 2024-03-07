// Copyright Philosophical Games


#include "Character/ValueCharacterBase.h"
#include "AbilitySystem/ValueAbilitySystemComponent.h"
#include "AbilitySystem/ValueAttributeSet.h"

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



