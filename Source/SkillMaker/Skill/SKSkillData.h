// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FSkillData.h"
#include "SKSkillData.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class SKILLMAKER_API USKSkillData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
	FSkillData SkillData;
};
