// Copyright Philosophical Games


#include "Character/ValueEnemy.h"
#include "AbilitySystem/ValueAbilitySystemComponent.h"
#include "AbilitySystem/ValueAttributeSet.h"
#include "Components/WidgetComponent.h"
#include "UI/Widget/ValueUserWidget.h"

AValueEnemy::AValueEnemy() { 
  GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECollisionResponse::ECR_Block);
  AbilitySystemComponent =
      CreateDefaultSubobject<UValueAbilitySystemComponent>(FName("AbilitySystemComponent"));
  AbilitySystemComponent->SetIsReplicated(true);
  AbilitySystemComponent->SetReplicationMode(
      EGameplayEffectReplicationMode::Minimal);
  AttributeSet = CreateDefaultSubobject<UValueAttributeSet>(FName("AttributeSet"));
  HealthBar = CreateDefaultSubobject<UWidgetComponent>(FName("HealthBar"));
  HealthBar->SetupAttachment(GetRootComponent());
}

int32 AValueEnemy::GetPlayerLevel() { return Level; }

void AValueEnemy::BeginPlay() { 
  Super::BeginPlay(); 
  InitAbilityActorInfo();
  if (UValueUserWidget* ValueUserWidget =
          Cast<UValueUserWidget>(HealthBar->GetUserWidgetObject())) {
    ValueUserWidget->SetWidgetController(this);
  }

  if (const UValueAttributeSet* AuraAS = Cast<UValueAttributeSet>(AttributeSet)) {
    AbilitySystemComponent
        ->GetGameplayAttributeValueChangeDelegate(AuraAS->GetHealthAttribute())
        .AddLambda([this](const FOnAttributeChangeData& Data) {
          OnHealthChanged.Broadcast(Data.NewValue);
        });
    AbilitySystemComponent
        ->GetGameplayAttributeValueChangeDelegate(
            AuraAS->GetMaxHealthAttribute())
        .AddLambda([this](const FOnAttributeChangeData& Data) {
          OnMaxHealthChanged.Broadcast(Data.NewValue);
        });

    OnHealthChanged.Broadcast(AuraAS->GetHealth());
    OnMaxHealthChanged.Broadcast(AuraAS->GetMaxHealth());
  }
}

void AValueEnemy::InitAbilityActorInfo() {
  AbilitySystemComponent->InitAbilityActorInfo(this, this);
  Cast<UValueAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();
  InitializeDefaultAttributes();
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
