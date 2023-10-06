// Copyright Philosophical Games


#include "Character/ValueCharacterBase.h"

AValueCharacterBase::AValueCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void AValueCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}



