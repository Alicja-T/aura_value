#pragma once
#include "GameplayEffectTypes.h"
#include "ValueAbilityTypes.generated.h"

USTRUCT(BlueprintType)
struct FValueGameplayEffectContext : public FGameplayEffectContext {
  GENERATED_BODY()

  public:
  bool IsCriticalHit() const { return bIsCriticalHit; }
  bool IsBlockedHit() const { return bIsBlockedHit; }

  void SetIsCriticalHit(bool bInIsCriticalHit) {
    bIsCriticalHit = bInIsCriticalHit;
  }
  void SetIsBlockedHit(bool bInIsBlockedHit) {
    bIsBlockedHit = bInIsBlockedHit;
  }

  /** Returns the actual struct used for serialization, subclasses must override
   * this! */
  virtual UScriptStruct* GetScriptStruct() const {
    return StaticStruct();
  }

  /** Creates a copy of this context, used to duplicate for later modifications
   */
  virtual FValueGameplayEffectContext* Duplicate() const {
    FValueGameplayEffectContext* NewContext = new FValueGameplayEffectContext();
    *NewContext = *this;
    if (GetHitResult()) {
      // Does a deep copy of the hit result
      NewContext->AddHitResult(*GetHitResult(), true);
    }
    return NewContext;
  }

  /** Custom serialization, subclasses must override this */
  virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map,
                            bool& bOutSuccess);

  protected:
  UPROPERTY()
  bool bIsBlockedHit = false;

  UPROPERTY()
  bool bIsCriticalHit = false;
};

template <>
struct TStructOpsTypeTraits<FValueGameplayEffectContext>
    : public TStructOpsTypeTraitsBase2<FValueGameplayEffectContext> {
  enum { WithNetSerializer = true, WithCopy = true };
};