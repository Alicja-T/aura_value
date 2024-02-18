// Copyright Philosophical Games


#include "Player/ValueController.h"
#include "EnhancedInputSubsystems.h"

AValueController::AValueController() { 
  bReplicates = true; 
}

void AValueController::BeginPlay() { 
  Super::BeginPlay();
  check(ValueContext);
  UEnhancedInputLocalPlayerSubsystem* Subsystem =
      ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
  check(Subsystem);
  Subsystem->AddMappingContext(ValueContext, 0);
  bShowMouseCursor = true;
  DefaultMouseCursor = EMouseCursor::Default;
  FInputModeGameAndUI InputModeData;
  InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
  InputModeData.SetHideCursorDuringCapture(false);
  SetInputMode(InputModeData);

}
