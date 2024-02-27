// Copyright Philosophical Games

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ValueEffectActor.generated.h"


class UGameplayEffect;
UCLASS()
class AURA_API AValueEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	

	AValueEffectActor();

protected:

	virtual void BeginPlay() override;
  UFUNCTION(BlueprintCallable)
  void ApplyEffectToTarget(AActor* TargetActor,
                         TSubclassOf<UGameplayEffect> GameplayEffect);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TSubclassOf<UGameplayEffect> InstantGameplayEffectClass;

private:


};
