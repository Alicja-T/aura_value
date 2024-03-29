// Copyright Philosophical Games

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/ValueWidgetController.h"
#include "AttributeMenuWidgetController.generated.h"


class UAttributeInfo;
struct FValueAttributeInfo;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoSignature,
                                            const FValueAttributeInfo&, Info);
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API UAttributeMenuWidgetController : public UValueWidgetController
{
	GENERATED_BODY()
 public:
   virtual void BroadcastInitialValues() override;
   virtual void BindCallbacksToDependencies() override;
   UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
   FAttributeInfoSignature AttributeInfoDelegate;

  protected:
   UPROPERTY(EditDefaultsOnly)
   TObjectPtr<UAttributeInfo> AttributeInfo;

  private:
   void BroadcastAttributeInfo(const FGameplayTag& AttributeTag,
                               const FGameplayAttribute& Attribute) const;
};
