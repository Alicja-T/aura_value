// Copyright Philosophical Games


#include "Actor/ValueProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"


AValueProjectile::AValueProjectile()
{
 	
	PrimaryActorTick.bCanEverTick = false;

  Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
  SetRootComponent(Sphere);
  Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
  Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
  Sphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
  Sphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
  Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

  ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(
      "ProjectileMovement");
  ProjectileMovement->InitialSpeed = 550.f;
  ProjectileMovement->MaxSpeed = 550.f;
  ProjectileMovement->ProjectileGravityScale = 0.f;
}

void AValueProjectile::BeginPlay(){

	Super::BeginPlay();
  Sphere->OnComponentBeginOverlap.AddDynamic(this,
                                             &AValueProjectile::OnSphereOverlap);
}

void AValueProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent,
                                       AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp,
                                       int32 OtherBodyIndex, bool bFromSweep,
                                       const FHitResult& SweepResult) {

}

