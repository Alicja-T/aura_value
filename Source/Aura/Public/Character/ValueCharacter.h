// Copyright Philosophical Games

#pragma once

#include "CoreMinimal.h"
#include "Character/ValueCharacterBase.h"
#include "Interaction/PlayerInterface.h"
#include "ValueCharacter.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AValueCharacter : public AValueCharacterBase, public IPlayerInterface
{
	GENERATED_BODY()
 public:
  AValueCharacter();
  virtual void PossessedBy(AController* Controller) override;
  virtual void OnRep_PlayerState() override;
  /* PlayerInterface */
  virtual void AddToXP_Implementation(int32 InXP) override;
  /* End PlayerInterface */


  /* CombatInterface */
  virtual int32 GetPlayerLevel() override;
  /* End CombatInterface */

 private:
  virtual void InitAbilityActorInfo() override;
 	
};
