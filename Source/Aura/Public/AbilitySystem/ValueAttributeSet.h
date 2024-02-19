// Copyright Philosophical Games

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "ValueAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UValueAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
	UPROPERTY()
  FGameplayAttributeData Health;
};
