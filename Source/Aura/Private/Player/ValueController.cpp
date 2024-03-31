// Copyright Philosophical Games

#include "Player/ValueController.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/ValueAbilitySystemComponent.h"
#include "Components/SplineComponent.h"
#include "EnhancedInputSubsystems.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "Input/ValueInputComponent.h"
#include "Interaction/EnemyInterface.h"
#include "ValueGameplayTags.h"

AValueController::AValueController() { 
  bReplicates = true; 
  Spline = CreateDefaultSubobject<USplineComponent>(FName("Spline"));
}

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
  // GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Red, *InputTag.ToString());
  if (InputTag.MatchesTagExact(FValueGameplayTags::Get().InputTag_LMB)) {
    bTargeting = CurrentActor ? true : false;
    bAutoRunning = false;
  }
}

void AValueController::AbilityInputTagReleased(FGameplayTag InputTag) {
  if (!InputTag.MatchesTagExact(FValueGameplayTags::Get().InputTag_LMB)) {
    if (GetASC()) {
      GetASC()->AbilityInputTagReleased(InputTag);
    }
    return;
  }

  if (bTargeting) {
    if (GetASC()) {
      GetASC()->AbilityInputTagReleased(InputTag);
    }
  } else {
    APawn* ControlledPawn = GetPawn();          
    if (FollowTime <= ShortPressThreshold && ControlledPawn) {
      if (UNavigationPath* NavPath =
              UNavigationSystemV1::FindPathToLocationSynchronously(
                  this, ControlledPawn->GetActorLocation(),
                  CachedDestination)) {
        Spline->ClearSplinePoints();
        for (const FVector& PointLoc : NavPath->PathPoints) {
          Spline->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);
          DrawDebugSphere(GetWorld(), PointLoc, 8.f, 8, FColor::Green, false,
                          5.f);

        }
        bAutoRunning = true;
      }
    }
    FollowTime = 0.f;
    bTargeting = false;
  }
}

void AValueController::AbilityInputTagHeld(FGameplayTag InputTag) {
  if (!InputTag.MatchesTagExact(FValueGameplayTags::Get().InputTag_LMB)) {
    if (GetASC()) {
      GetASC()->AbilityInputTagHeld(InputTag);
    }
    return;
  }

  if (bTargeting) {
    if (GetASC()) {
      GetASC()->AbilityInputTagHeld(InputTag);
    }
  } else {
    FollowTime += GetWorld()->GetDeltaSeconds();

    FHitResult Hit;
    if (GetHitResultUnderCursor(ECC_Visibility, false, Hit)) {
      CachedDestination = Hit.ImpactPoint;
    }

    if (APawn* ControlledPawn = GetPawn()) {
      const FVector WorldDirection =
          (CachedDestination - ControlledPawn->GetActorLocation())
              .GetSafeNormal();
      ControlledPawn->AddMovementInput(WorldDirection);
    }
  }
}

UValueAbilitySystemComponent* AValueController::GetASC() { 
  if (ValueAbilitySystemComponent == nullptr) {
    ValueAbilitySystemComponent = Cast<UValueAbilitySystemComponent>(
        UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(
            GetPawn<APawn>()));
  }
  return ValueAbilitySystemComponent;
}
