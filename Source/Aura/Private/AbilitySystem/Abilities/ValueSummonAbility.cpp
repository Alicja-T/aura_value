// Copyright Philosophical Games


#include "AbilitySystem/Abilities/ValueSummonAbility.h"
#include <Kismet/KismetSystemLibrary.h>

TArray<FVector> UValueSummonAbility::GetSpawnLocations() {
  const FVector Forward =
      GetAvatarActorFromActorInfo()->GetActorForwardVector();
  const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
  const float DeltaSpread = SpawnSpread / NumMinions;

  const FVector LeftOfSpread =
      Forward.RotateAngleAxis(-SpawnSpread / 2.f, FVector::UpVector);
  TArray<FVector> SpawnLocations;
  for (int32 i = 0; i < NumMinions; i++) {
    const FVector Direction =
        LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
    FVector ChosenSpawnLocation =
        Location +
        Direction * FMath::FRandRange(MinSpawnDistance, MaxSpawnDistance);
    FHitResult HitResult;
    GetWorld()->LineTraceSingleByChannel(
        HitResult, ChosenSpawnLocation + FVector(0.f, 0.f, 500.f),
        ChosenSpawnLocation - FVector(0.f, 0.f, 500.f), ECC_Visibility);
    if (HitResult.bBlockingHit) {
      ChosenSpawnLocation = HitResult.ImpactPoint;
    }
    SpawnLocations.Add(ChosenSpawnLocation);
  }

  return SpawnLocations;
}

TSubclassOf<APawn> UValueSummonAbility::GetRandomMinionClass() {
  const int32 Index = FMath::RandRange(0, MinionClasses.Num() - 1);
  return MinionClasses[Index];
}
