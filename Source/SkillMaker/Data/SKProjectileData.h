#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "SKProjectileData.generated.h"

class ASKProjectileActor;

USTRUCT(BlueprintType)
struct FSKProjectileData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ProjectileName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Description;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UTexture2D> Thumbnail;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftClassPtr<ASKProjectileActor> ProjectileActor;
};
