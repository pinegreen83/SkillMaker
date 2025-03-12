#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "SKWeaponData.generated.h"

USTRUCT(BlueprintType)
struct FSKWeaponData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString WeaponName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* WeaponThumbnail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString WeaponType;
};
