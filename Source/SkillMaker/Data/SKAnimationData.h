#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"
#include "SKAnimationData.generated.h"

USTRUCT(BlueprintType)
struct FSKAnimationData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString AnimationName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (Categories = "Weapon"))
	FGameplayTagContainer CompatibleWeaponTags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UTexture2D> AnimationThumbnail;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UAnimMontage> AnimationMontage;
};
