// Copyright Philosophical Games

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "ValueInputComponent.generated.h"
#include "ValueInputConfig.h"

/**
 *
 */
UCLASS()
class AURA_API UValueInputComponent : public UEnhancedInputComponent {
  GENERATED_BODY()

 public:
  template <class UserClass, typename PressedFuncType,
            typename ReleasedFuncType, typename HeldFuncType>
  void BindAbilityActions(const UValueInputConfig*, UserClass* Object,
                          PressedFuncType PressedFunc,
                          ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc);
};

template <class UserClass, typename PressedFuncType, typename ReleasedFuncType,
          typename HeldFuncType>
inline void UValueInputComponent::BindAbilityActions(
    const UValueInputConfig*, UserClass* Object, PressedFuncType PressedFunc,
    ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc) {
  check(InputConfig);
  for (const auto& Action : InputConfig->AbilityInputActions) {
    if (Action.InputAction && Action.InputTag.IsValid()) {
      if (PressedFunc) {
        BindAction(Action.InputAction, ETriggerEvent::Started, Object,
                   PressedFunc, Action.InputTag);
      }

      if (ReleasedFunc) {
        BindAction(Action.InputAction, ETriggerEvent::Completed, Object,
                   ReleasedFunc, Action.InputTag);
      }

      if (HeldFunc) {
        BindAction(Action.InputAction, ETriggerEvent::Triggered, Object,
                   HeldFunc, Action.InputTag);
      }
    }
  }
  
}
