#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "SKAnimationData.generated.h"

USTRUCT(BlueprintType)
struct FSKAnimationData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString AnimationName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* AnimationMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* AnimationThumbnail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString WeaponType;
};
