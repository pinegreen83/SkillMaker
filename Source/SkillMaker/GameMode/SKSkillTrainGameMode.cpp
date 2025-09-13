// Fill out your copyright notice in the Description page of Project Settings.


#include "SKSkillTrainGameMode.h"
#include "UI/UI-SkillMaker/TrainingRoom/SKSkillMakerTrainHUD.h"

ASKSkillTrainGameMode::ASKSkillTrainGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassRef(TEXT("/Script/Engine.Blueprint'/Game/SkillMaker/Blueprint/Character/BP_SKPlayerCharacter.BP_SKPlayerCharacter_C'"));
	if(DefaultPawnClassRef.Class != nullptr)
	{
		DefaultPawnClass = DefaultPawnClassRef.Class;
	}
	
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/Engine.Blueprint'/Game/SkillMaker/Blueprint/Controller/BP_SKPlayerController.BP_SKPlayerController_C'"));
	if(PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}

	HUDClass = ASKSkillMakerTrainHUD::StaticClass();
}