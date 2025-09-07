// Fill out your copyright notice in the Description page of Project Settings.


#include "SKSkillSelectionWidget.h"
#include "Components/UniformGridPanel.h"
#include "Skill/SKSkillData.h"
#include "Skill/SKSkillManager.h"
#include "SKSkillCardWidget.h"
#include "Logging/SKLogSkillMakerMacro.h"

void USKSkillSelectionWidget::NativeConstruct()
{
	Super::NativeConstruct();
	LoadSkillList();
}

void USKSkillSelectionWidget::LoadSkillList()
{
	SK_LOG(LogSkillMaker, Log, TEXT("스킬 목록을 불러옵니다."));

	SkillList = USKSkillManager::Get()->GetSkillList();

	int32 index = 0;
	for(const auto& Skill : SkillList)
	{
		SK_LOG(LogSkillMaker, Log, TEXT("스킬 로드 : %s"), *Skill.SkillName);

		CreateSkillCard(Skill.SkillID, Skill.SkillName, nullptr, index);
		index++;
	}
}

void USKSkillSelectionWidget::CreateSkillCard(const FName& SkillID, const FString& SkillName, UTexture2D* Thumbnail, const int32 SkillIndex)
{
	if(!SkillGridPanel || !WBP_SKSkillCard)
		return;

	if(USKSkillCardWidget* SkillCard = CreateWidget<USKSkillCardWidget>(GetWorld(), WBP_SKSkillCard))
	{
		SkillCard->SetSkillInfo(SkillID, SkillName, Thumbnail);
		SkillCard->OnSkillCardSelected.AddDynamic(this, &USKSkillSelectionWidget::SelectSkill);
		SkillGridPanel->AddChildToUniformGrid(SkillCard, SkillIndex/4, SkillIndex%4);
	}
}

void USKSkillSelectionWidget::SelectSkill(const FName& SkillID)
{
	SK_LOG(LogSkillMaker, Log, TEXT("선택된 스킬 : %s"), *SkillID.ToString());

	OnSkillSelected.Broadcast(SkillID);
}
