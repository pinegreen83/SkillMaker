// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SKSKillMakerHUD.generated.h"

class USKSkillPreviewWidget;
class ASKPreviewCharacter;

UCLASS()
class SKILLMAKER_API ASKSKillMakerHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<USKSkillPreviewWidget> SkillPreviewWidgetClass;

	UPROPERTY()
	USKSkillPreviewWidget* SkillPreviewWidget;

	UPROPERTY()
	ASKPreviewCharacter* PreviewCharacter;
};
