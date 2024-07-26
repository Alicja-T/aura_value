// Copyright Philosophical Games


#include "Character/ValueEnemy.h"
#include "AbilitySystem/ValueAbilitySystemComponent.h"
#include "AbilitySystem/ValueAttributeSet.h"
#include "AbilitySystem/ValueAbilitySystemLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "AI/ValueAIController.h"
#include "Components/WidgetComponent.h"
#include "UI/Widget/ValueUserWidget.h"
#include "ValueGameplayTags.h"
#include "GameFramework/CharacterMovementComponent.h"

AValueEnemy::AValueEnemy() { 
  GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECollisionResponse::ECR_Block);
  AbilitySystemComponent =
      CreateDefaultSubobject<UValueAbilitySystemComponent>(FName("AbilitySystemComponent"));
  AbilitySystemComponent->SetIsReplicated(true);
  AbilitySystemComponent->SetReplicationMode(
      EGameplayEffectReplicationMode::Minimal);
  bUseControllerRotationPitch = false;
  bUseControllerRotationRoll = false;
  bUseControllerRotationYaw = false;
  GetCharacterMovement()->bUseControllerDesiredRotation = true;
  AttributeSet = CreateDefaultSubobject<UValueAttributeSet>(FName("AttributeSet"));
  HealthBar = CreateDefaultSubobject<UWidgetComponent>(FName("HealthBar"));
  HealthBar->SetupAttachment(GetRootComponent());
}

void AValueEnemy::PossessedBy(AController* NewController) {
  Super::PossessedBy(NewController);
  if (!HasAuthority()) return;
  ValueAIController = Cast<AValueAIController>(NewController);
  ValueAIController->GetBlackboardComponent()->InitializeBlackboard(*(BehaviorTree->BlackboardAsset));
  ValueAIController->RunBehaviorTree(BehaviorTree);
  ValueAIController->GetBlackboardComponent()->SetValueAsBool(
      FName("HitReacting"), false);
  ValueAIController->GetBlackboardComponent()->SetValueAsBool(
      FName("RangedAttacker"), CharacterClass != ECharacterClass::Warrior);
}

int32 AValueEnemy::GetPlayerLevel() { return Level; }

void AValueEnemy::Die() { 
  SetLifeSpan(LifeSpan);
  Super::Die(); 
 }

void AValueEnemy::SetCombatTarget_Implementation(AActor* InCombatTarget) {
   CombatTarget = InCombatTarget;
 }

 AActor* AValueEnemy::GetCombatTarget_Implementation() const { 
   return CombatTarget; 
 }

void AValueEnemy::HitReactTagChanged(const FGameplayTag CallbackTag,
                                     int32 NewCount) {
  bHitReacting = NewCount > 0;
  GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;
  if (ValueAIController && ValueAIController->GetBlackboardComponent()) {
  ValueAIController->GetBlackboardComponent()->SetValueAsBool(
      FName("HitReacting"), bHitReacting);
  }
}

void AValueEnemy::BeginPlay() { 
  Super::BeginPlay(); 
  GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
  InitAbilityActorInfo();
  if (HasAuthority()) {
    UValueAbilitySystemLibrary::GiveStartupAbilities(this,
                                                     AbilitySystemComponent, CharacterClass);
  }
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
    AbilitySystemComponent
        ->RegisterGameplayTagEvent(FValueGameplayTags::Get().Effects_HitReact,
                                   EGameplayTagEventType::NewOrRemoved)
        .AddUObject(this, &AValueEnemy::HitReactTagChanged);

    OnHealthChanged.Broadcast(AuraAS->GetHealth());
    OnMaxHealthChanged.Broadcast(AuraAS->GetMaxHealth());
  }
}

void AValueEnemy::InitAbilityActorInfo() {
  AbilitySystemComponent->InitAbilityActorInfo(this, this);
  Cast<UValueAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();
  if (HasAuthority()) {
    InitializeDefaultAttributes();
  }
}

void AValueEnemy::InitializeDefaultAttributes() const {
  UValueAbilitySystemLibrary::InitializeDefaultAttributes(
      this, CharacterClass, Level, AbilitySystemComponent);
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
