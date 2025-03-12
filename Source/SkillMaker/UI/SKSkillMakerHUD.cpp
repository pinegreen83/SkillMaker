// Fill out your copyright notice in the Description page of Project Settings.


#include "SKSkillMakerHUD.h"
#include "SKSkillMakerMainWidget.h"
#include "Engine/Texture2D.h"

ASKSkillMakerHUD::ASKSkillMakerHUD()
{
	static ConstructorHelpers::FClassFinder<USKSkillMakerMainWidget> MainWidgetClassFinder(TEXT("/Game/SkillMaker/UI/WBP_SkillMakerMain.WBP_SkillMakerMain_C"));
	if(MainWidgetClassFinder.Class)
	{
		MainWidgetClass = MainWidgetClassFinder.Class;
	}
}

void ASKSkillMakerHUD::BeginPlay()
{
	Super::BeginPlay();

	if(MainWidgetClass)
	{
		MainWidget = CreateWidget<USKSkillMakerMainWidget>(GetWorld(), MainWidgetClass);
		if(MainWidget)
		{
			MainWidget->AddToViewport();
		}
	}
}
