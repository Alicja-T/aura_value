// Copyright Philosophical Games

#pragma once

#include "CoreMinimal.h"
#include "Character/ValueCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "ValueEnemy.generated.h"


/**
 * 
 */
UCLASS()
class AURA_API AValueEnemy : public AValueCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()
 public:
  virtual void HighlightActor() override;
  virtual void UnHighlightActor() override;

};
