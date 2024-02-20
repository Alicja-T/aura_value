// Copyright Philosophical Games

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ValueWidgetController.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UValueWidgetController : public UObject
{
	GENERATED_BODY()
 protected:
  UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
  TObjectPtr<APlayerController> PlayerController;
  UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
  TObjectPtr<APlayerState> PlayerState;
  UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
  TObjectPtr<class UAbilitySystemComponent> AbilitySystemComponent;
  UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
  TObjectPtr<class UAttributeSet> AttributeSet;
	
};
