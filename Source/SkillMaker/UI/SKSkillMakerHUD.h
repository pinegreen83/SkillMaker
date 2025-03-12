// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SKSkillMakerHUD.generated.h"

class USKSkillMakerMainWidget;

UCLASS()
class SKILLMAKER_API ASKSkillMakerHUD : public AHUD
{
	GENERATED_BODY()

public:
	ASKSkillMakerHUD();
	
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<USKSkillMakerMainWidget> MainWidgetClass;

	UPROPERTY()
	USKSkillMakerMainWidget* MainWidget;
};
