// Copyright Philosophical Games


#include "AbilitySystem/ValueAbilitySystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HUD/ValueHUD.h"
#include "Player/ValuePlayerState.h"
#include "UI/WidgetController/ValueWidgetController.h"

UOverlayWidgetController*
UValueAbilitySystemLibrary::GetOverlayWidgetController(
    const UObject* WorldContextObject) {
  if (APlayerController* PC =
          UGameplayStatics::GetPlayerController(WorldContextObject, 0)) {
    if (AValueHUD* HUD = Cast<AValueHUD>(PC->GetHUD())) {
      AValuePlayerState* PS = PC->GetPlayerState<AValuePlayerState>(); 
      UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
      UAttributeSet* AS = PS->GetAttributeSet();
      const FWidgetControllerParams WidgetContollerParams(PC, PS, ASC, AS);
      return HUD->GetOverlayWidgetController(WidgetContollerParams);
    }
  }
  return nullptr;
}
