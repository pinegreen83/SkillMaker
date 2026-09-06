// Fill out your copyright notice in the Description page of Project Settings.


#include "SKSkillMakerEditorMainWidget.h"
#include "UI/UI-SkillMaker/Common/SkillSelect/SKSkillSelectionWidget.h"
#include "UI/UI-SkillMaker/Common/AnimationSelect/SKAnimationSelectionWidget.h"
#include "UI/UI-SkillMaker/Common/SkillSelect/SKSkillDetailWidget.h"
#include "UI/UI-SkillMaker/Common/WeaponSelect/SKWeaponSelectionWidget.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "SKSkillMakerEditorHUD.h"
#include "Logging/SKLogSkillMakerMacro.h"

bool USKSkillMakerEditorMainWidget::Initialize()
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));

	if(!Super::Initialize())
		return false;

	PreviousStates.Add(ESKSkillMakerState::ChooseAction);

	if(SkillSelectionWidget)
	{
		SkillSelectionWidget->SetEditorMainWidget(this);
		SkillSelectionWidget->OnSkillSelected.AddDynamic(this, &USKSkillMakerEditorMainWidget::OnSkillSelected);
	}

	if(WeaponSelectionWidget)
	{
		WeaponSelectionWidget->OnWeaponSelected.AddDynamic(this, &USKSkillMakerEditorMainWidget::OnWeaponSelected);
	}

	if(AnimationSelectionWidget)
	{
		AnimationSelectionWidget->OnAnimationSelected.AddDynamic(this, &USKSkillMakerEditorMainWidget::OnAnimationSelected);
	}

	if(SkillDetailWidget)
	{
		SkillDetailWidget->SetSkillMakerEditorHUD(HUDReference);
	}

	if(ModifySkillButton)
	{
		ModifySkillButton->OnClicked.AddDynamic(this, &USKSkillMakerEditorMainWidget::OnModifySkillClicked);
	}

	if(CreateSkillButton)
	{
		CreateSkillButton->OnClicked.AddDynamic(this, &USKSkillMakerEditorMainWidget::OnCreateSkillClicked);
	}

	if(FinishEditingSkillButton)
	{
		FinishEditingSkillButton->OnClicked.AddDynamic(this, &USKSkillMakerEditorMainWidget::OnFinishSkillEditing);
	}

	if(SaveSkillButton)
	{
		SaveSkillButton->OnClicked.AddDynamic(this, &USKSkillMakerEditorMainWidget::OnSaveSkillClicked);
	}

	if(BackButton)
	{
		BackButton->OnClicked.AddDynamic(this, &USKSkillMakerEditorMainWidget::OnBackClicked);
	}

	return true;
}

void USKSkillMakerEditorMainWidget::SetHUDReference(ASKSkillMakerEditorHUD* InHUD)
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));
	if (HUDReference)
	{
		if (WeaponSelectionWidget)
		{
			WeaponSelectionWidget->OnWeaponSelected.RemoveDynamic(HUDReference, &ASKSkillMakerEditorHUD::SetSkillWeaponTag);
		}
		if (AnimationSelectionWidget)
		{
			AnimationSelectionWidget->OnAnimationSelected.RemoveDynamic(HUDReference, &ASKSkillMakerEditorHUD::SetSkillMontage);
		}
	}

	HUDReference = InHUD;
	if (!HUDReference)
	{
		return;
	}

	if (WeaponSelectionWidget)
	{
		WeaponSelectionWidget->OnWeaponSelected.RemoveDynamic(this, &USKSkillMakerEditorMainWidget::OnWeaponSelected);
		WeaponSelectionWidget->OnWeaponSelected.AddUniqueDynamic(HUDReference, &ASKSkillMakerEditorHUD::SetSkillWeaponTag);
		WeaponSelectionWidget->OnWeaponSelected.AddUniqueDynamic(this, &USKSkillMakerEditorMainWidget::OnWeaponSelected);
	}

	if (AnimationSelectionWidget)
	{
		AnimationSelectionWidget->OnAnimationSelected.RemoveDynamic(this, &USKSkillMakerEditorMainWidget::OnAnimationSelected);
		AnimationSelectionWidget->OnAnimationSelected.AddUniqueDynamic(HUDReference, &ASKSkillMakerEditorHUD::SetSkillMontage);
		AnimationSelectionWidget->OnAnimationSelected.AddUniqueDynamic(this, &USKSkillMakerEditorMainWidget::OnAnimationSelected);
	}

	if(SkillDetailWidget)
	{
		SkillDetailWidget->SetSkillMakerEditorHUD(HUDReference);
	}
}

void USKSkillMakerEditorMainWidget::SetSkillMakerState(ESKSkillMakerState NewState, bool bFromBackNavigation)
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

	if (HUDReference)
	{
		const FSKSkillData& CurrentSkill = HUDReference->GetCurrentSkillData();
		if (NewState == ESKSkillMakerState::ChooseWeapon && WeaponSelectionWidget)
		{
			WeaponSelectionWidget->LoadWeaponList(CurrentSkill.WeaponTag);
		}
		else if (NewState == ESKSkillMakerState::ChooseAnimation && AnimationSelectionWidget)
		{
			AnimationSelectionWidget->LoadAnimationsForWeapon(CurrentSkill.WeaponTag, CurrentSkill.SkillMontage);
		}
		else if (NewState == ESKSkillMakerState::SkillDetail && SkillDetailWidget)
		{
			SkillDetailWidget->InitializeFromSkillData();
		}
	}
}

void USKSkillMakerEditorMainWidget::GoBackToPreviousState()
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

void USKSkillMakerEditorMainWidget::OnModifySkillClicked()
{
	SK_LOG(LogSkillMaker, Log, TEXT("기존 스킬 수정 시작"));
	OnSkillDataFromTable.Broadcast();
	SetSkillMakerState(ESKSkillMakerState::ChooseSkill, false);
}

void USKSkillMakerEditorMainWidget::OnCreateSkillClicked()
{
	SK_LOG(LogSkillMaker, Log, TEXT("새로운 스킬 생성 시작"));
	if (HUDReference)
	{
		HUDReference->InitializeNewSkill();
	}
	SetSkillMakerState(ESKSkillMakerState::ChooseWeapon, false);
}

void USKSkillMakerEditorMainWidget::OnSkillSelected(const FName& SkillID)
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));

	if(!HUDReference)
	{
		SK_LOG(LogSkillMaker, Error, TEXT("HUD 레퍼런스 없음."));
		return;
	}

	HUDReference->LoadSkillForEditing(SkillID);
	SK_LOG(LogSkillMaker, Log, TEXT("스킬 선택됨 : %s"), *SkillID.ToString());

	SetSkillMakerState(ESKSkillMakerState::SkillDetail, false);
}

void USKSkillMakerEditorMainWidget::OnWeaponSelected(FGameplayTag WeaponTag)
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));

	if (!WeaponTag.IsValid())
	{
		SK_LOG(LogSkillMaker, Warning, TEXT("유효하지 않은 무기 태그가 선택됨."));
		return;
	}

	SK_LOG(LogSkillMaker, Log, TEXT("선택된 무기 : %s"), *WeaponTag.ToString());

	SetSkillMakerState(ESKSkillMakerState::ChooseAnimation, false);
}

void USKSkillMakerEditorMainWidget::OnAnimationSelected(const TSoftObjectPtr<UAnimMontage>& AnimationMontage)
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));

	if(!AnimationMontage.IsNull())
	{
		SK_LOG(LogSkillMaker, Log, TEXT("선택된 애니메이션 : %s"), *AnimationMontage.ToSoftObjectPath().ToString());
		SetSkillMakerState(ESKSkillMakerState::SkillDetail, false);
	}
	else
	{
		SK_LOG(LogSkillMaker, Warning, TEXT("애니메이션이 선택되지 않음."));
	}
}

void USKSkillMakerEditorMainWidget::OnFinishSkillEditing()
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));

	SetSkillMakerState(ESKSkillMakerState::SaveSkill, false);
}

void USKSkillMakerEditorMainWidget::OnSaveSkillClicked()
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

	if (!HUDReference->SaveCurrentSkill(SkillName))
	{
		return;
	}
	OnSkillDataFromTable.Broadcast();

	SetSkillMakerState(ESKSkillMakerState::ChooseAction, false);
}

void USKSkillMakerEditorMainWidget::OnBackClicked()
{
	GoBackToPreviousState();
}
