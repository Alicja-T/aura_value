// Copyright Philosophical Games


#include "Player/ValueController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

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

void AValueController::SetupInputComponent() { 
  Super::SetupInputComponent(); 
  UEnhancedInputComponent* EnhancedInputComponent =
      CastChecked<UEnhancedInputComponent>(InputComponent);
  EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this,
                                     &AValueController::Move);
}

void AValueController::Move(const FInputActionValue& InputActionValue) {
  const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
  const FRotator Rotation = GetControlRotation();
  const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);
  const FVector ForwardDirection =
      FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
  const FVector RightDirection =
      FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

  if (APawn* ControlledPawn = GetPawn()) {
    ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
    ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
  
  }
}
