// Fill out your copyright notice in the Description page of Project Settings.


#include "Prop/SKSkillStation.h"
#include "Character/SKPlayerCharacter.h"
#include "Player/SKPlayerController.h"
#include "UI/UI-SkillMaker/TrainingRoom/SKSkillMakerTrainHUD.h"


void ASKSkillStation::OnInteract()
{
	Super::OnInteract();

	if (!CurrentPlayerCharacter)
	{
		return;
	}

	if (ASKPlayerController* PlayerController = Cast<ASKPlayerController>(CurrentPlayerCharacter->GetController()))
	{
		if (ASKSkillMakerTrainHUD* TrainHUD = Cast<ASKSkillMakerTrainHUD>(PlayerController->GetHUD()))
		{
			TrainHUD->ShowSkillSelection();
		}
	}
}
