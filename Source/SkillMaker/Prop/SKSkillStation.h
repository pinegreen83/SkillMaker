// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Prop/SKInteractableActor.h"
#include "SKSkillStation.generated.h"

/**
 * 
 */
UCLASS()
class SKILLMAKER_API ASKSkillStation : public ASKInteractableActor
{
	GENERATED_BODY()

public:
	virtual void OnInteract() override;

protected:
	UPROPERTY(EditAnywhere, Category = "SkillMaker")
	TSubclassOf<class UUserWidget> SkillEditorWidgetClass;
};
