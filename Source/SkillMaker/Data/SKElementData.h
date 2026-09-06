#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SKElementData.generated.h"

USTRUCT(BlueprintType)
struct FSKElementData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Element")
	FGameplayTag ElementTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Element")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Element")
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Element")
	FLinearColor DisplayColor = FLinearColor::White;
};
