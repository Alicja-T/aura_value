// Copyright Philosophical Games

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "ValuePlayerState.generated.h"


class UAbilitySystemComponent;
class UAttributeSet;
class ULevelUpInfo;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChanged, int32)

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
  UPROPERTY(EditDefaultsOnly)
  TObjectPtr<ULevelUpInfo> LevelUpInfo;
  FOnPlayerStatChanged OnXPChangedDelegate;
  FOnPlayerStatChanged OnLevelChangedDelegate;

  FORCEINLINE int32 GetPlayerLevel() const { return Level; }
  FORCEINLINE int32 GetPlayerXP() const { return XP; }
  void AddToXP(int32 inXP);
  void AddToLevel(int32 InLevel);
  void SetXP(int32 inXP);
  void SetLevel(int32 InLevel);

 protected:
   UPROPERTY(VisibleAnywhere)
   TObjectPtr<class UAbilitySystemComponent> AbilitySystemComponent;
   UPROPERTY()
   TObjectPtr<class UAttributeSet> AttributeSet; 
 private:
   UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_Level)
   int32 Level = 1;

   UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_XP)
   int32 XP = 1;
   UFUNCTION()
   void OnRep_Level(int32 OldLevel);

   UFUNCTION()
   void OnRep_XP(int32 OldXP);
	
};
