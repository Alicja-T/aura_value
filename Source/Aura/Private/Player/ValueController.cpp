// Copyright Philosophical Games

#include "Player/ValueController.h"

#include "EnhancedInputSubsystems.h"
#include "Input/ValueInputComponent.h"
#include "Interaction/EnemyInterface.h"

AValueController::AValueController() { bReplicates = true; }

void AValueController::PlayerTick(float DeltaTime) {
  Super::PlayerTick(DeltaTime);
  CursorTrace();
}

void AValueController::BeginPlay() {
  Super::BeginPlay();
  check(ValueContext);
  UEnhancedInputLocalPlayerSubsystem* Subsystem =
      ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
          GetLocalPlayer());
  if (Subsystem) {
    Subsystem->AddMappingContext(ValueContext, 0);
  }
  bShowMouseCursor = true;
  DefaultMouseCursor = EMouseCursor::Default;
  FInputModeGameAndUI InputModeData;
  InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
  InputModeData.SetHideCursorDuringCapture(false);
  SetInputMode(InputModeData);
}

void AValueController::SetupInputComponent() {
  Super::SetupInputComponent();
  UValueInputComponent* ValueInputComponent =
      CastChecked<UValueInputComponent>(InputComponent);
  ValueInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this,
                                  &AValueController::Move);
  ValueInputComponent->BindAbilityActions(
      InputConfig, this, &ThisClass::AbilityInputTagPressed,
      &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
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

void AValueController::CursorTrace() {
  FHitResult CursorHit;
  GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
  if (!CursorHit.bBlockingHit) return;
  LastActor = CurrentActor;
  CurrentActor = Cast<IEnemyInterface>(CursorHit.GetActor());
  if (LastActor == nullptr) {
    if (CurrentActor != nullptr) {
      CurrentActor->HighlightActor();
    }
  } else {
    if (CurrentActor == nullptr) {
      LastActor->UnHighlightActor();
    } else if (LastActor != CurrentActor) {
      LastActor->UnHighlightActor();
      CurrentActor->HighlightActor();
    }
  }
}

void AValueController::AbilityInputTagPressed(FGameplayTag InputTag) {
  GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Red, *InputTag.ToString());
}

void AValueController::AbilityInputTagReleased(FGameplayTag InputTag) {
  GEngine->AddOnScreenDebugMessage(2, 3.f, FColor::Blue, *InputTag.ToString());
}

void AValueController::AbilityInputTagHeld(FGameplayTag InputTag) {
  GEngine->AddOnScreenDebugMessage(3, 3.f, FColor::Green, *InputTag.ToString());
}
