// Copyright Philosophical Games

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/ValueWidgetController.h"
#include "OverlayWidgetController.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, float,
                                    NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxHealthChangedSignature, float,
                                    NewMaxHealth);
 /**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API UOverlayWidgetController : public UValueWidgetController
{
	GENERATED_BODY()
 public:
   virtual void BroadcastInitialValues() override;
   virtual void BindCallbacksToDependencies() override;
  UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
  FOnHealthChangedSignature OnHealthChanged;

  UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
  FOnMaxHealthChangedSignature OnMaxHealthChanged;

 protected:
  void HealthChanged(const FOnAttributeChangeData& Data) const;
  void MaxHealthChanged(const FOnAttributeChangeData& Data) const;
};
