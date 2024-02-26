// Copyright Philosophical Games


#include "Actor/ValueEffectActor.h"

// Sets default values
AValueEffectActor::AValueEffectActor() {
  PrimaryActorTick.bCanEverTick = false;
  SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot")));
}


void AValueEffectActor::BeginPlay() {
	Super::BeginPlay();

}





