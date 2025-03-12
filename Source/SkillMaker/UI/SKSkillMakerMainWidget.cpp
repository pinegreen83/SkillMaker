// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SKSkillMakerMainWidget.h"
#include "SKAnimationSelectionWidget.h"
#include "SKWeaponSelectionWidget.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"

bool USKSkillMakerMainWidget::Initialize()
{
	bool Success = Super::Initialize();
	if(!Success)
		return false;

	if(WeaponSelectionWidget)
	{
		WeaponSelectionWidget->OnWeaponSelected.AddDynamic(this, &USKSkillMakerMainWidget::OnWeaponSelected);
	}
	
	if(ModifySkillButton)
	{
		ModifySkillButton->OnClicked.AddDynamic(this, &USKSkillMakerMainWidget::OnModifySkillClicked);
	}

	if(CreateSkillButton)
	{
		CreateSkillButton->OnClicked.AddDynamic(this, &USKSkillMakerMainWidget::OnCreateSkillClicked);
	}

	if(SaveSkillButton)
	{
		SaveSkillButton->OnClicked.AddDynamic(this, &USKSkillMakerMainWidget::OnSaveSkillClicked);
	}

	if(BackButton)
	{
		BackButton->OnClicked.AddDynamic(this, &USKSkillMakerMainWidget::OnBackClicked);
	}

	return true;
}

void USKSkillMakerMainWidget::SetSkillMakerState(ESKSkillMakerState NewState)
{
	CurrentState = NewState;

	if(SkillMakerSwitcher)
	{
		SkillMakerSwitcher->SetActiveWidgetIndex(static_cast<int32>(NewState));
	}

	if(BackButton)
	{
		BackButton->SetVisibility(CurrentState == ESKSkillMakerState::ChooseAction
									? ESlateVisibility::Collapsed
									: ESlateVisibility::Visible);
	}
}

void USKSkillMakerMainWidget::OnModifySkillClicked()
{
	UE_LOG(LogTemp, Log, TEXT("기존 스킬 수정 기능은 비활성화됨."));

	if(ModifySkillButton)
	{
		ModifySkillButton->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void USKSkillMakerMainWidget::OnCreateSkillClicked()
{
	UE_LOG(LogTemp, Log, TEXT("새로운 스킬 생성 시작"));
	SetSkillMakerState(ESKSkillMakerState::ChooseWeapon);
}

void USKSkillMakerMainWidget::OnWeaponSelected(const FString& WeaponType)
{
	SelectedWeaponType = WeaponType;
	UE_LOG(LogTemp, Log, TEXT("선택된 무기 : %s"), *SelectedWeaponType);

	if(AnimationSelectionWidget)
	{
		AnimationSelectionWidget->LoadAnimationsForWeapon(WeaponType);
	}

	SetSkillMakerState(ESKSkillMakerState::ChooseAnimation);
}

void USKSkillMakerMainWidget::OnSaveSkillClicked()
{
	FString SkillName = SkillNameInput->GetText().ToString();

	if(SkillName.IsEmpty())
	{
		UE_LOG(LogTemp, Log, TEXT("스킬 이름이 입력되지 않음."));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("스킬 %s 저장 완료"), *SkillName);
	SetSkillMakerState(ESKSkillMakerState::ChooseAction);
}

void USKSkillMakerMainWidget::OnBackClicked()
{
	switch(CurrentState)
	{
	case ESKSkillMakerState::ChooseWeapon:
		SetSkillMakerState(ESKSkillMakerState::ChooseAction);
		break;
	case ESKSkillMakerState::ChooseAnimation:
		SetSkillMakerState(ESKSkillMakerState::ChooseWeapon);
		break;
	case ESKSkillMakerState::EditSkill:
		SetSkillMakerState(ESKSkillMakerState::ChooseAnimation);
		break;
	case ESKSkillMakerState::SaveSkill:
		SetSkillMakerState(ESKSkillMakerState::EditSkill);
		break;
	default:
		break;
	}
}
