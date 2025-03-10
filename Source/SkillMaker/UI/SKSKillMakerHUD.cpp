// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SKSKillMakerHUD.h"
#include "Blueprint/UserWidget.h"
#include "SKSkillPreviewWidget.h"
#include "Character/SKPreviewCharacter.h"
#include "Kismet/GameplayStatics.h"

void ASKSKillMakerHUD::BeginPlay()
{
	Super::BeginPlay();

	if(SkillPreviewWidgetClass)
	{
		SkillPreviewWidget = CreateWidget<USKSkillPreviewWidget>(GetWorld(), SkillPreviewWidgetClass);
		if(SkillPreviewWidget)
		{
			SkillPreviewWidget->AddToViewport();

			PreviewCharacter = Cast<ASKPreviewCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
			if(PreviewCharacter)
			{
				SkillPreviewWidget->SetPreviewCharacter(PreviewCharacter);
			}
		}
	}
}
