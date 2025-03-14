// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SKSkillTrainGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SKILLMAKER_API ASKSkillTrainGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASKSkillTrainGameMode();

	virtual void BeginPlay() override;
};
