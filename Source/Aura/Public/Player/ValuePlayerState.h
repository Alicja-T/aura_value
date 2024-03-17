// Copyright Philosophical Games

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "ValuePlayerState.generated.h"


class UAbilitySystemComponent;
class UAttributeSet;

/**
 * 
 */
UCLASS()
class AURA_API AValuePlayerState : public APlayerState, public IAbilitySystemInterface 
{
	GENERATED_BODY()
 public:
   AValuePlayerState();
  virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
   virtual void GetLifetimeReplicatedProps(
       TArray<FLifetimeProperty>& OutLifetimeProps) const override;
  UAttributeSet* GetAttributeSet() const { return AttributeSet; }
  FORCEINLINE int32 GetPlayerLevel() const { return Level; }

 protected:
   UPROPERTY(VisibleAnywhere)
   TObjectPtr<class UAbilitySystemComponent> AbilitySystemComponent;
   UPROPERTY()
   TObjectPtr<class UAttributeSet> AttributeSet; 
 private:
   UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_Level)
   int32 Level = 1;
   UFUNCTION()
   void OnRep_Level(int32 OldLevel);
	
};
