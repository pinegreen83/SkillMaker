// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SKSkillPreviewWidget.h"
#include "Components/Button.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "Character/SKPreviewCharacter.h"

bool USKSkillPreviewWidget::Initialize()
{
	bool Success = Super::Initialize();
	if(!Success) return false;
	
	if(PlayButton)
	{
		PlayButton->OnClicked.AddDynamic(this, &USKSkillPreviewWidget::OnPlayButtonClicked);
	}

	if(ScrubSlider)
	{
		ScrubSlider->OnValueChanged.AddDynamic(this, &USKSkillPreviewWidget::OnScrubValueChanged);
	}

	return true;
}

void USKSkillPreviewWidget::SetPreviewCharacter(ASKPreviewCharacter* InPreviewCharacter)
{
	PreviewCharacter = InPreviewCharacter;
}

void USKSkillPreviewWidget::OnPlayButtonClicked()
{
	if(PreviewCharacter && PreviewCharacter->GetPreviewMontage())
	{
		PreviewCharacter->PlayPreviewAnimation(PreviewCharacter->GetPreviewMontage());
	}
}

void USKSkillPreviewWidget::OnScrubValueChanged(float Value)
{
	if(PreviewCharacter && PreviewCharacter->GetPreviewMontage())
	{
		float MontageLength = PreviewCharacter->GetPreviewMontage()->GetPlayLength();
		float NewTime = MontageLength * Value;
		PreviewCharacter->SetPreviewAnimationTime(NewTime);
	}
}
