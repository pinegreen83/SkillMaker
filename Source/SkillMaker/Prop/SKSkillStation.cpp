// Fill out your copyright notice in the Description page of Project Settings.


#include "Prop/SKSkillStation.h"
#include "UI/UI-SkillMaker/TrainingRoom/SKSkillMakerTrainMainWidget.h"


void ASKSkillStation::OnInteract()
{
	Super::OnInteract();

	if (SkillMakerTrainMainWidgetClass)
	{
		if (USKSkillMakerTrainMainWidget* SkillTrainMain = CreateWidget<USKSkillMakerTrainMainWidget>(GetWorld(), SkillMakerTrainMainWidgetClass))
		{
			if (CurrentPlayerCharacter)
			{
				SkillTrainMain->StartSkillMaker(CurrentPlayerCharacter);
				SkillTrainMain->AddToViewport(0);
			}
		}
	}
}
