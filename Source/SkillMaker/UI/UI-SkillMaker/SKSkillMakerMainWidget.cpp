// Fill out your copyright notice in the Description page of Project Settings.


#include "SKSkillMakerMainWidget.h"
#include "SKSkillSelectionWidget.h"
#include "SKAnimationSelectionWidget.h"
#include "SKSkillDetailWidget.h"
#include "SKWeaponSelectionWidget.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "SKSkillMakerHUD.h"
#include "Logging/SKLogSkillMakerMacro.h"
#include "Skill/SKSkillManager.h"

bool USKSkillMakerMainWidget::Initialize()
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));
	
	bool Success = Super::Initialize();
	if(!Success)
		return false;

	PreviousStates.Add(ESKSkillMakerState::ChooseAction);
	
	if(SkillSelectionWidget)
	{
		SkillSelectionWidget->OnSkillSelected.AddDynamic(this, &USKSkillMakerMainWidget::OnSkillSelected);
	}
	
	if(WeaponSelectionWidget)
	{
		WeaponSelectionWidget->OnWeaponSelected.AddDynamic(this, &USKSkillMakerMainWidget::OnWeaponSelected);
	}

	if(AnimationSelectionWidget)
	{
		AnimationSelectionWidget->OnAnimationSelected.AddDynamic(this, &USKSkillMakerMainWidget::OnAnimationSelected);
	}

	if(SkillDetailWidget)
	{
		SkillDetailWidget->SetHUDReference(HUDReference);
	}
	
	if(ModifySkillButton)
	{
		ModifySkillButton->OnClicked.AddDynamic(this, &USKSkillMakerMainWidget::OnModifySkillClicked);
	}

	if(CreateSkillButton)
	{
		CreateSkillButton->OnClicked.AddDynamic(this, &USKSkillMakerMainWidget::OnCreateSkillClicked);
	}

	if(FinishEditingSkillButton)
	{
		FinishEditingSkillButton->OnClicked.AddDynamic(this, &USKSkillMakerMainWidget::OnFinishSkillEditing);
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

void USKSkillMakerMainWidget::SetHUDReference(ASKSkillMakerHUD* InHUD)
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));
	
	HUDReference = InHUD;

	if(SkillDetailWidget)
	{
		SkillDetailWidget->SetHUDReference(HUDReference);
	}
}

void USKSkillMakerMainWidget::SetSkillMakerState(ESKSkillMakerState NewState, bool bFromBackNavigation)
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));
	
	if(!SkillMakerSwitcher)
	{
		SK_LOG(LogSkillMaker, Error, TEXT("WidgetSwitcher가 nullptr"));
		return;
	}

	if (static_cast<int32>(NewState) >= SkillMakerSwitcher->GetNumWidgets())
	{
		SK_LOG(LogSkillMaker, Error, TEXT("잘못된 WidgetSwitcher 인덱스 NewState : %d, 총 위젯 개수 : %d"),
			   static_cast<int32>(NewState), SkillMakerSwitcher->GetNumWidgets());
		return;
	}

	if (!bFromBackNavigation && CurrentState != NewState && NewState != ESKSkillMakerState::ChooseAction)
	{
		PreviousStates.Add(CurrentState);
	}

	if (BackButton)
	{
		BackButton->SetVisibility(PreviousStates.Num() > 1 ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}

	CurrentState = NewState;
	SkillMakerSwitcher->SetActiveWidgetIndex(static_cast<int32>(NewState));
}

void USKSkillMakerMainWidget::GoBackToPreviousState()
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));
	
	if(PreviousStates.Num() <= 1)
	{
		SK_LOG(LogSkillMaker, Error, TEXT("이전 상태가 없으므로 뒤로 갈 수 없음."));
		return;
	}

	ESKSkillMakerState PreviousState = PreviousStates.Last();
	PreviousStates.RemoveAt(PreviousStates.Num() - 1);

	SetSkillMakerState(PreviousState, true);
}

void USKSkillMakerMainWidget::OnModifySkillClicked()
{
	SK_LOG(LogSkillMaker, Log, TEXT("기존 스킬 수정 시작"));
	SetSkillMakerState(ESKSkillMakerState::ChooseSkill, false);
}

void USKSkillMakerMainWidget::OnCreateSkillClicked()
{
	SK_LOG(LogSkillMaker, Log, TEXT("새로운 스킬 생성 시작"));
	SetSkillMakerState(ESKSkillMakerState::ChooseWeapon, false);
}

void USKSkillMakerMainWidget::OnSkillSelected(const FName& SkillID)
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));
	
	if(!HUDReference)
	{
		SK_LOG(LogSkillMaker, Error, TEXT("HUD 레퍼런스 없음."));
		return;
	}
	
	HUDReference->LoadSkillForEditing(SkillID);
	SK_LOG(LogSkillMaker, Log, TEXT("스킬 선택됨 : %s"), *SkillID.ToString());

	if(SkillDetailWidget)
	{
		SkillDetailWidget->InitializeFromSkillData();
	}

	SetSkillMakerState(ESKSkillMakerState::SkillDetail, false);
}

void USKSkillMakerMainWidget::OnWeaponSelected(const FString& WeaponType)
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));
	
	if(!HUDReference)
	{
		SK_LOG(LogSkillMaker, Error, TEXT("HUD 레퍼런스 없음."));
		return;
	}
	
	SelectedWeaponType = WeaponType;
	SK_LOG(LogSkillMaker, Log, TEXT("선택된 무기 : %s"), *SelectedWeaponType);
	
	HUDReference->SetSkillWeaponType(WeaponType);
	
	if(AnimationSelectionWidget)
	{
		AnimationSelectionWidget->LoadAnimationsForWeapon(WeaponType);
	}

	SetSkillMakerState(ESKSkillMakerState::ChooseAnimation, false);
}

void USKSkillMakerMainWidget::OnAnimationSelected(UAnimMontage* AnimationMontage)
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));
	
	if(!HUDReference)
	{
		SK_LOG(LogSkillMaker, Error, TEXT("HUD 레퍼런스 없음."));
		return;
	}

	if(AnimationMontage)
	{
		HUDReference->SetSkillMontage(AnimationMontage);
		SK_LOG(LogSkillMaker, Log, TEXT("선택된 애니메이션 : %s"), *AnimationMontage->GetName());

		if(SkillDetailWidget)
		{
			SkillDetailWidget->InitializeFromSkillData();
		}
		
		SetSkillMakerState(ESKSkillMakerState::SkillDetail, false);
	}
	else
	{
		SK_LOG(LogSkillMaker, Warning, TEXT("애니메이션이 선택되지 않음."));
	}
}

void USKSkillMakerMainWidget::OnFinishSkillEditing()
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));
	
	SetSkillMakerState(ESKSkillMakerState::SaveSkill, false);
}

void USKSkillMakerMainWidget::OnSaveSkillClicked()
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));
	
	if (!HUDReference)
	{
		SK_LOG(LogSkillMaker, Error, TEXT("HUD 레퍼런스 없음."));
		return;
	}

	FString SkillName = SkillNameInput->GetText().ToString();
	if (SkillName.IsEmpty())
	{
		SK_LOG(LogSkillMaker, Error, TEXT("스킬 이름이 입력되지 않음."));
		return;
	}

	HUDReference->SetSkillName(SkillName);
	SK_LOG(LogSkillMaker, Log, TEXT("스킬 %s 저장 진행 중..."), *SkillName);

	FSKSkillData SkillData = HUDReference->GetCurrentSkillData();

	if (SkillData.SkillID.IsNone())
	{
		SkillData.SkillID = FName(*FGuid::NewGuid().ToString());
		SK_LOG(LogSkillMaker, Log, TEXT("새로운 SkillID 생성: %s"), *SkillData.SkillID.ToString());
	}
	USKSkillManager::Get()->SaveSkillData(SkillData);
	
	SK_LOG(LogSkillMaker, Log, TEXT("스킬 저장 완료: %s"), *SkillName);

	SetSkillMakerState(ESKSkillMakerState::ChooseAction, false);
}

void USKSkillMakerMainWidget::OnBackClicked()
{
	GoBackToPreviousState();
}
