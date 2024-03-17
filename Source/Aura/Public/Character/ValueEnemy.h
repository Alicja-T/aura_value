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
  AValueEnemy();
  /* EnemyInterface */
  virtual void HighlightActor() override;
  virtual void UnHighlightActor() override;
  /* End EnemyInterface */

  /* CombatInterface */
  virtual int32 GetPlayerLevel() override;
  /* End CombatInterface */

 protected:
  virtual void BeginPlay() override;
  virtual void InitAbilityActorInfo() override;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Interface")
  int32 Level = 1;
};
