// Copyright Philosophical Games


#include "Character/ValueCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Pawn.h"
#include "Player/ValuePlayerState.h"
#include "Player/ValueController.h"
#include "UI/HUD/ValueHUD.h"
#include "AbilitySystem/ValueAbilitySystemComponent.h"

AValueCharacter::AValueCharacter() { 
  GetCharacterMovement()->bOrientRotationToMovement = true;
  GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
  GetCharacterMovement()->bConstrainToPlane = true;
  GetCharacterMovement()->bSnapToPlaneAtStart = true;

  bUseControllerRotationPitch = false;
  bUseControllerRotationRoll = false;
  bUseControllerRotationYaw = false;
}

void AValueCharacter::PossessedBy(AController* ValueController) {
  Super::PossessedBy(ValueController);
  InitAbilityActorInfo();
}



void AValueCharacter::OnRep_PlayerState() { 
  Super::OnRep_PlayerState(); 
  InitAbilityActorInfo();
}



void AValueCharacter::InitAbilityActorInfo() {
  AValuePlayerState* ValuePlayerState = GetPlayerState<AValuePlayerState>();
  check(ValuePlayerState);
  AbilitySystemComponent = ValuePlayerState->GetAbilitySystemComponent();
  AbilitySystemComponent->InitAbilityActorInfo(ValuePlayerState, this);
  Cast<UValueAbilitySystemComponent>(ValuePlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();
  AttributeSet = ValuePlayerState->GetAttributeSet();
  if (AValueController* ValuePlayerController =
          Cast<AValueController>(GetController())) {
    if (AValueHUD* ValueHUD = Cast<AValueHUD>(ValuePlayerController->GetHUD())) {
      ValueHUD->InitOverlay(ValuePlayerController, ValuePlayerState, AbilitySystemComponent, AttributeSet);
    }
  };
  InitializePrimaryAttributes();
}