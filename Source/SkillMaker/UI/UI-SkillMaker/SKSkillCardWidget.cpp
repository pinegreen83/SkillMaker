// Fill out your copyright notice in the Description page of Project Settings.


#include "SKSkillCardWidget.h"
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

void USKSkillCardWidget::SetSkillInfo(const FName& SkillID, const FString& SkillName, UTexture2D* Thumbnail)
{
	StoredSkillID = SkillID;
	StoredSkillName = SkillName;

	UE_LOG(LogTemp, Log, TEXT("스킬 아이디 : %s, 스킬 이름 : %s"), *SkillID.ToString(), *SkillName);
	
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
	OnSkillCardSelected.Broadcast(StoredSkillID);
}
