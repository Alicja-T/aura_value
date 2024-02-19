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
 UFUNCTION()
 virtual void OnOverlap(UPrimitiveComponent* OverlappedComponent,
                              AActor* OtherActor,
                              UPrimitiveComponent* OtherComp,
                              int32 OtherBodyIndex, bool bFromSweep,
                              const FHitResult& SweepResult);
 UFUNCTION()
 virtual void OnStopOverlap(UPrimitiveComponent* OverlappedComponent,
                                  AActor* OtherActor,
                                  UPrimitiveComponent* OtherComp,
                                  int32 OtherBodyIndex);

private:
  UPROPERTY(VisibleAnywhere)
  TObjectPtr<USphereComponent> Sphere;
  UPROPERTY(VisibleAnywhere)
  TObjectPtr<UStaticMeshComponent> Mesh;

};
