// Copyright Philosophical Games


#include "Actor/ValueEffectActor.h"
#include "Components/SphereComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/ValueAttributeSet.h"

// Sets default values
AValueEffectActor::AValueEffectActor()
{
 
	PrimaryActorTick.bCanEverTick = false;
  Mesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Mesh"));
  SetRootComponent(Mesh);
  Sphere = CreateDefaultSubobject<USphereComponent>(FName("Sphere"));
  Sphere->SetupAttachment(GetRootComponent());
}


void AValueEffectActor::BeginPlay()
{
	Super::BeginPlay();
  Sphere->OnComponentBeginOverlap.AddDynamic(this, &AValueEffectActor::OnOverlap);
  Sphere->OnComponentEndOverlap.AddDynamic(this, &AValueEffectActor::OnStopOverlap);
}

void AValueEffectActor::OnOverlap(UPrimitiveComponent* OverlappedComponent,
                                  AActor* OtherActor,
                                  UPrimitiveComponent* OtherComp,
                                  int32 OtherBodyIndex, bool bFromSweep,
                                  const FHitResult& SweepResult) {
  // TODO: Change this to apply a Gameplay Effect. For now, using const_cast as
  // a hack!
  if (IAbilitySystemInterface* ASCInterface =
          Cast<IAbilitySystemInterface>(OtherActor)) {
    const UValueAttributeSet* ValueAttributeSet = Cast<UValueAttributeSet>(ASCInterface->GetAbilitySystemComponent()->GetAttributeSet(
        UValueAttributeSet::StaticClass()));
    UValueAttributeSet* ChangingAttributeSet =
        const_cast<UValueAttributeSet*>(ValueAttributeSet);
    ChangingAttributeSet->SetHealth(ValueAttributeSet->GetHealth() + 25.f);
    Destroy();
  }

}

void AValueEffectActor::OnStopOverlap(UPrimitiveComponent* OverlappedComponent,
                                      AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp,
                                      int32 OtherBodyIndex) {}



