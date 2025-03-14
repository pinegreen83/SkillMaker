// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SKSkillCardWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

bool USKSkillCardWidget::Initialize()
{
	bool Success = Super::Initialize();
	if(!Success)
		return false;

	if(SkillButton)
	{
		SkillButton->OnClicked.AddDynamic(this, &USKSkillCardWidget::HandleSkillSelected);
	}

	return true;
}

void USKSkillCardWidget::SetSkillInfo(const FString& SkillName, UTexture2D* Thumbnail)
{
	StoredSkillName = SkillName;
	
	if(SkillNameText)
	{
		SkillNameText->SetText(FText::FromString(SkillName));
	}

	if(SkillThumbnail)
	{
		SkillThumbnail->SetBrushFromTexture(Thumbnail);
	}
}

void USKSkillCardWidget::HandleSkillSelected()
{
	OnSkillCardSelected.Broadcast(StoredSkillName);
}
