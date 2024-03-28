// Copyright Philosophical Games

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/ValueWidgetController.h"
#include "AttributeMenuWidgetController.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAttributeMenuWidgetController : public UValueWidgetController
{
	GENERATED_BODY()
 public:
   virtual void BroadcastInitialValues() override;
   virtual void BindCallbacksToDependencies() override;
};
