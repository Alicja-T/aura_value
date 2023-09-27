// Copyright Philosophical Games

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ValueCharacterBase.generated.h"

UCLASS()
class AURA_API AValueCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AValueCharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
