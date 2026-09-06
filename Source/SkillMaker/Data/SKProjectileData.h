#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"
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

	/** 비어 있으면 모든 속성과 호환되는 레거시 데이터로 취급 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Element"))
	FGameplayTagContainer SupportedElementTags;
};
