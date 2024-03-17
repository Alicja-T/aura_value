// Copyright Philosophical Games


#include "Character/ValueEnemy.h"
#include "AbilitySystem/ValueAbilitySystemComponent.h"
#include "AbilitySystem/ValueAttributeSet.h"

AValueEnemy::AValueEnemy() { 
  GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECollisionResponse::ECR_Block);
  AbilitySystemComponent =
      CreateDefaultSubobject<UValueAbilitySystemComponent>(FName("AbilitySystemComponent"));
  AbilitySystemComponent->SetIsReplicated(true);
  AbilitySystemComponent->SetReplicationMode(
      EGameplayEffectReplicationMode::Minimal);
  AttributeSet = CreateDefaultSubobject<UValueAttributeSet>(FName("AttributeSet"));
}

int32 AValueEnemy::GetPlayerLevel() { return Level; }

void AValueEnemy::BeginPlay() { 
  Super::BeginPlay(); 
  InitAbilityActorInfo();
}

void AValueEnemy::InitAbilityActorInfo() {
  AbilitySystemComponent->InitAbilityActorInfo(this, this);
  Cast<UValueAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();
}

void AValueEnemy::HighlightActor() { 
  GetMesh()->SetRenderCustomDepth(true);
  GetMesh()->SetCustomDepthStencilValue(250);
  Weapon->SetRenderCustomDepth(true);
  Weapon->SetCustomDepthStencilValue(250);
}

void AValueEnemy::UnHighlightActor() { 
  GetMesh()->SetRenderCustomDepth(false);
  Weapon->SetRenderCustomDepth(false);
}
