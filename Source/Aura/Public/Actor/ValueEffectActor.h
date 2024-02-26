// Copyright Philosophical Games

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ValueEffectActor.generated.h"


class USphereComponent;
UCLASS()
class AURA_API AValueEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	

	AValueEffectActor();

protected:

	virtual void BeginPlay() override;

private:


};
