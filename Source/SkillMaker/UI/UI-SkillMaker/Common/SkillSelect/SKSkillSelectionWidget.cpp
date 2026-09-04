// Fill out your copyright notice in the Description page of Project Settings.


#include "SKSkillSelectionWidget.h"
#include "Components/UniformGridPanel.h"
#include "Game/SKSaveGameSubsystem.h"
#include "Skill/SKSkillData.h"
#include "SKSkillCardWidget.h"
#include "UI/UI-SkillMaker/UEEditor/SKSkillMakerEditorMainWidget.h"
#include "UI/UI-SkillMaker/TrainingRoom/SKSkillMakerTrainHUD.h"
#include "Logging/SKLogSkillMakerMacro.h"

void USKSkillSelectionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	LoadSkillList();
}

void USKSkillSelectionWidget::SetEditorMainWidget(USKSkillMakerEditorMainWidget* Main)
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));
	
	EditorMainWidget = Main;
	
	if (EditorMainWidget)
	{
		EditorMainWidget->OnSkillDataFromTable.AddDynamic(this, &USKSkillSelectionWidget::LoadSkillList);
	}
	else
	{
		SK_LOG(LogSkillMaker, Log, TEXT("No MainWidget"));
	}
}

void USKSkillSelectionWidget::SetTrainMainWidget(USKSkillMakerTrainMainWidget* Main)
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));

	TrainMainWidget = Main;

	if (TrainMainWidget)
	{
		
	}
}

void USKSkillSelectionWidget::LoadSkillList()
{
	SK_LOG(LogSkillMaker, Log, TEXT("스킬 목록을 불러옵니다."));

	if (!SkillGridPanel)
	{
		SK_LOG(LogSkillMaker, Error, TEXT("SkillGridPanel이 nullptr임."));
		return;
	}

	SkillGridPanel->ClearChildren();

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		if (USKSaveGameSubsystem* SaveGameSubsystem = GameInstance->GetSubsystem<USKSaveGameSubsystem>())
		{
			SkillList = SaveGameSubsystem->GetSavedSkillList();
		}
		else
		{
			SK_LOG(LogSkillMaker, Error, TEXT("USKSaveGameSubsystem을 찾을 수 없음."));
			SkillList.Empty();
		}
	}
	else
	{
		SK_LOG(LogSkillMaker, Error, TEXT("GameInstance를 찾을 수 없음."));
		SkillList.Empty();
	}

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
