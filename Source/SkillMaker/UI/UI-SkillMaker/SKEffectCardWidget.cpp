// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UI-SkillMaker/SKEffectCardWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Logging/SKLogSkillMakerMacro.h"

bool USKEffectCardWidget::Initialize()
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));
	
	if (!Super::Initialize()) return false;

	if (EffectSelectButton)
	{
		EffectSelectButton->OnClicked.AddDynamic(this, &USKEffectCardWidget::OnEffectButtonClicked);
	}

	return true;
}

void USKEffectCardWidget::SetEffectInfo(TSubclassOf<AActor> InEffectClass)
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));
	
	EffectClass = InEffectClass;

	if (EffectNameText && EffectClass)
	{
		EffectNameText->SetText(FText::FromString(EffectClass->GetName()));
	}
}

void USKEffectCardWidget::OnEffectButtonClicked()
{
	OnEffectSelected.Broadcast(EffectClass);
}
