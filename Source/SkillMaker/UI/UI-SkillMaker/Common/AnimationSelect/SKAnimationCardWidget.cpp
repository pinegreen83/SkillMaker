// Fill out your copyright notice in the Description page of Project Settings.


#include "SKAnimationCardWidget.h"
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

bool USKAnimationCardWidget::SetAnimationInfo(const FString& AnimationName, const TSoftObjectPtr<UAnimMontage>& Montage, const TSoftObjectPtr<UTexture2D>& Thumbnail, bool bIsSelected)
{
	if(Montage.IsNull())
	{
		SelectedMontage.Reset();
		return false;
	}

	if(AnimationNameText)
	{
		const FString DisplayName = AnimationName.IsEmpty() ? Montage.GetAssetName() : AnimationName;
		AnimationNameText->SetText(FText::FromString(DisplayName));
	}

	if(AnimationThumbnail)
	{
		AnimationThumbnail->SetBrushFromSoftTexture(Thumbnail);
	}

	SelectedMontage = Montage;
	if (AnimationButton)
	{
		AnimationButton->SetBackgroundColor(bIsSelected ? FLinearColor(0.15f, 0.55f, 1.0f, 1.0f) : FLinearColor::White);
	}
	return true;
}

void USKAnimationCardWidget::HandleAnimationSelected()
{
	if(SelectedMontage.IsNull())
	{
		return;
	}
	
	OnAnimationCardSelected.Broadcast(SelectedMontage);
}
