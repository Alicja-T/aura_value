// Copyright Philosophical Games

#pragma once

#include "CoreMinimal.h"
#include "Character/ValueCharacterBase.h"
#include "ValueCharacter.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AValueCharacter : public AValueCharacterBase
{
	GENERATED_BODY()
 public:
  AValueCharacter();
  virtual void PossessedBy(AController* Controller) override;
  virtual void OnRep_PlayerState() override;
 private:
  void InitAbilitySystem();
 	
};
