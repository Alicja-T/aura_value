// Copyright Philosophical Games

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ValueAbilitySystemLibrary.generated.h"

class UOverlayWidgetController;
/**
 * 
 */
UCLASS()
class AURA_API UValueAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
 public:
   UFUNCTION(BlueprintPure, Category = "ValueAbilitySystemLibrary|WidgetController")
   static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);
};
