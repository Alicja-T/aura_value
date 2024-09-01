// Copyright Philosophical Games

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LevelUpInfo.generated.h"


USTRUCT(BlueprintType)
struct FValueLevelUpInfo {

	GENERATED_BODY()

	  UPROPERTY(EditDefaultsOnly)
    int32 LevelUpRequirement = 0;

		UPROPERTY(BlueprintReadOnly)
    int32 Level;

		UPROPERTY(EditDefaultsOnly)
    int32 AttributePointsReward = 1;

		UPROPERTY(EditDefaultsOnly)
    int32 SpellPointsReward = 1;


};

/**
 * 
 */
UCLASS()
class AURA_API ULevelUpInfo : public UDataAsset
{
	GENERATED_BODY()

	public:
    UPROPERTY(EditDefaultsOnly)
		TArray<FValueLevelUpInfo> LevelUpInformation;

		int32 FindLevelForXP(const int32 XP) const;
	
};
