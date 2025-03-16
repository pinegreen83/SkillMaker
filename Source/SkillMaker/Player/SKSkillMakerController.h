// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SKSkillMakerController.generated.h"

/**
 * 
 */
UCLASS()
class SKILLMAKER_API ASKSkillMakerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
};
