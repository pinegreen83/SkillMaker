// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UI-SkillMaker/SKProjectileCardWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Logging/SKLogSkillMakerMacro.h"

bool USKProjectileCardWidget::Initialize()
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));
	
	if (!Super::Initialize()) return false;

	if (ProjectileSelectButton)
	{
		ProjectileSelectButton->OnClicked.AddDynamic(this, &USKProjectileCardWidget::OnProjectileButtonClicked);
	}

	return true;
}

void USKProjectileCardWidget::SetProjectileInfo(const TSubclassOf<ASKProjectileActor> InProjectileClass)
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));
	
	ProjectileClass = InProjectileClass;

	if (ProjectileNameText && ProjectileClass)
	{
		ProjectileNameText->SetText(FText::FromString(ProjectileClass->GetName()));
	}
}

void USKProjectileCardWidget::OnProjectileButtonClicked()
{
	OnProjectileCardSelected.Broadcast(ProjectileClass);
}
