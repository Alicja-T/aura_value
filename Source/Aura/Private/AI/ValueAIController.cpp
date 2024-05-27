// Copyright Philosophical Games


#include "AI/ValueAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

AValueAIController::AValueAIController() {
  Blackboard = CreateDefaultSubobject<UBlackboardComponent>(FName("BlackboardComponent"));
  check(Blackboard);
  BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(
      FName("BehaviorTreeComponent"));
  check(BehaviorTreeComponent);
}
