// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SKGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SKILLMAKER_API ASKGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASKGameMode();

protected:
	virtual void BeginPlay() override;
};
