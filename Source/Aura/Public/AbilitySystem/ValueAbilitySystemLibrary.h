// Copyright Philosophical Games

#pragma once

#include "CoreMinimal.h"
#include "Data/CharacterClassInfo.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ValueAbilitySystemLibrary.generated.h"

class UOverlayWidgetController;
class UAttributeMenuWidgetController;
class UAbilitySystemComponent;
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

   UFUNCTION(BlueprintPure,
             Category = "ValueAbilitySystemLibrary|WidgetController")
   static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(
       const UObject* WorldContextObject);
   UFUNCTION(BlueprintCallable,
             Category = "ValueAbilitySystemLibrary|CharacterClassDefaults")
   static void InitializeDefaultAttributes(
       const UObject* WorldContextObject, ECharacterClass CharacterClass,
                                           float level,
                                           UAbilitySystemComponent* ASC);

   UFUNCTION(BlueprintCallable,
             Category = "ValueAbilitySystemLibrary|CharacterClassDefaults")
   static void GiveStartupAbilities(const UObject* WorldContextObject,
                                    UAbilitySystemComponent* ASC);
};
