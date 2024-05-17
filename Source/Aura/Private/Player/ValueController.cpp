// Copyright Philosophical Games

#include "Player/ValueController.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/ValueAbilitySystemComponent.h"
#include "Components/SplineComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Character.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "Input/ValueInputComponent.h"
#include "Interaction/EnemyInterface.h"
#include "UI/Widget/DamageTextComponent.h"
#include "ValueGameplayTags.h"


AValueController::AValueController() { 
  bReplicates = true; 
  Spline = CreateDefaultSubobject<USplineComponent>(FName("Spline"));
}

void AValueController::PlayerTick(float DeltaTime) {
  Super::PlayerTick(DeltaTime);
  CursorTrace();
  AutoRun();
}

void AValueController::ShowDamageNumber_Implementation(
    float DamageAmount, ACharacter* TargetCharacter, bool bBlockedHit,
    bool bCriticalHit) {
  if (IsValid(TargetCharacter) && DamageTextComponentClass) {
    UDamageTextComponent* DamageTextComponent = NewObject<UDamageTextComponent>(TargetCharacter, DamageTextComponentClass);
    DamageTextComponent->RegisterComponent();
    DamageTextComponent->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
    DamageTextComponent->DetachFromComponent(
        FDetachmentTransformRules::KeepWorldTransform);
    DamageTextComponent->SetDamageText(DamageAmount, bBlockedHit, bCriticalHit);
  }

}

void AValueController::AutoRun() {
  if (!bAutoRunning) return;
  if (APawn* ControlledPawn = GetPawn()) {
    const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(
        ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
    const FVector Direction = Spline->FindDirectionClosestToWorldLocation(
        LocationOnSpline, ESplineCoordinateSpace::World);
    ControlledPawn->AddMovementInput(Direction);
    const float DistanceToDestination =
        (LocationOnSpline - CachedDestination).Length();
    if (DistanceToDestination <= AutoRunAcceptanceRadius) {
      bAutoRunning = false;
    }
  }
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
  ValueInputComponent->BindAction(LeftShiftAction, ETriggerEvent::Started, this,
                                 &AValueController::LeftShiftPressed);
  ValueInputComponent->BindAction(LeftShiftAction, ETriggerEvent::Completed, this,
                                 &AValueController::LeftShiftReleased);
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
  GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
  if (!CursorHit.bBlockingHit) return;
  LastActor = CurrentActor;
  CurrentActor = Cast<IEnemyInterface>(CursorHit.GetActor());
  if (LastActor != CurrentActor) {
    if (LastActor) {
      LastActor->UnHighlightActor();
    }
    if (CurrentActor) {
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

  if (GetASC()) GetASC()->AbilityInputTagReleased(InputTag);

  if (!bTargeting && !bLeftShiftDown) {
    const APawn* ControlledPawn = GetPawn();          
    if (FollowTime <= ShortPressThreshold && ControlledPawn) {
      if (UNavigationPath* NavPath =
              UNavigationSystemV1::FindPathToLocationSynchronously(
                  this, ControlledPawn->GetActorLocation(),
                  CachedDestination)) {
        Spline->ClearSplinePoints();
        for (const FVector& PointLoc : NavPath->PathPoints) {
          Spline->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);
        }
        CachedDestination = NavPath->PathPoints[NavPath->PathPoints.Num() - 1];
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

	

	if (bTargeting || bLeftShiftDown) {
    if (GetASC()) GetASC()->AbilityInputTagHeld(InputTag);
  } else {
    FollowTime += GetWorld()->GetDeltaSeconds();
    if (CursorHit.bBlockingHit) CachedDestination = CursorHit.ImpactPoint;
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


