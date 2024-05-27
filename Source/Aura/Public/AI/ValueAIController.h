// Copyright Philosophical Games

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ValueAIController.generated.h"

class UBlackboardComponent;
class UBehaviorTreeComponent;

/**
 * 
 */
UCLASS()
class AURA_API AValueAIController : public AAIController
{
	GENERATED_BODY()
 public:
   AValueAIController();

 protected:
 	
   UPROPERTY()
   TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;
};
