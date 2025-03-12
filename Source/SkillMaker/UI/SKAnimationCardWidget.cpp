// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SKAnimationCardWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

bool USKAnimationCardWidget::Initialize()
{
	bool Success = Super::Initialize();
	if(!Success)
		return false;

	if(AnimationButton)
	{
		AnimationButton->OnClicked.AddDynamic(this, &USKAnimationCardWidget::HandleAnimationSelected);
	}

	return true;
}

void USKAnimationCardWidget::SetAnimationInfo(UAnimMontage* Montage, UTexture2D* Thumbnail)
{
	if(!Montage)
		return;

	if(AnimationNameText)
	{
		AnimationNameText->SetText(FText::FromString(Montage->GetName()));
	}

	if(AnimationThumbnail)
	{
		AnimationThumbnail->SetBrushFromTexture(Thumbnail);
	}
}

void USKAnimationCardWidget::HandleAnimationSelected()
{
	OnAnimationSelected.Broadcast(nullptr);
}
