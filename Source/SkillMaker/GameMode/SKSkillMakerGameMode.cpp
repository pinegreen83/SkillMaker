// Fill out your copyright notice in the Description page of Project Settings.


#include "SKSkillMakerGameMode.h"
#include "UI/UI-SkillMaker/UEEditor/SKSkillMakerEditorHUD.h"

ASKSkillMakerGameMode::ASKSkillMakerGameMode()
{
	HUDClass = ASKSkillMakerEditorHUD::StaticClass();
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/Engine.Blueprint'/Game/SkillMaker/Blueprint/Controller/BP_SKSkillMakerController.BP_SKSkillMakerController_C'"));
	if(PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}
}

void ASKSkillMakerGameMode::BeginPlay()
{
	Super::BeginPlay();
}
