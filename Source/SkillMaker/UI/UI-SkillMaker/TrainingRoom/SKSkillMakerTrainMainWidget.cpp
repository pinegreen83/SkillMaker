// Fill out your copyright notice in the Description page of Project Settings.


#include "SKSkillMakerTrainMainWidget.h"
#include "UI/UI-SkillMaker/Common/SkillSelect/SKSkillSelectionWidget.h"
#include "UI/UI-SkillMaker/Common/AnimationSelect/SKAnimationSelectionWidget.h"
#include "UI/UI-SkillMaker/Common/SkillSelect/SKSkillDetailWidget.h"
#include "UI/UI-SkillMaker/Common/WeaponSelect/SKWeaponSelectionWidget.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "Logging/SKLogSkillMakerMacro.h"

bool USKSkillMakerTrainMainWidget::Initialize()
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));
	
	bool Success = Super::Initialize();
	if(!Success)
		return false;

	PreviousStates.Add(ESKSkillMakerTrainState::ChooseAction);
	
	if(SkillSelectionWidget)
	{
		// SkillSelectionWidget->SetMainWidget(this);
		SkillSelectionWidget->OnSkillSelected.AddDynamic(this, &USKSkillMakerTrainMainWidget::OnSkillSelected);
	}
	
	if(WeaponSelectionWidget)
	{
		WeaponSelectionWidget->OnWeaponSelected.AddDynamic(this, &USKSkillMakerTrainMainWidget::OnWeaponSelected);
	}

	if(AnimationSelectionWidget)
	{
		AnimationSelectionWidget->OnAnimationSelected.AddDynamic(this, &USKSkillMakerTrainMainWidget::OnAnimationSelected);
	}

	if(SkillDetailWidget)
	{
		// SkillDetailWidget->SetHUDReference(HUDReference);
	}
	
	if(ModifySkillButton)
	{
		ModifySkillButton->OnClicked.AddDynamic(this, &USKSkillMakerTrainMainWidget::OnModifySkillClicked);
	}

	if(CreateSkillButton)
	{
		CreateSkillButton->OnClicked.AddDynamic(this, &USKSkillMakerTrainMainWidget::OnCreateSkillClicked);
	}

	if(FinishEditingSkillButton)
	{
		FinishEditingSkillButton->OnClicked.AddDynamic(this, &USKSkillMakerTrainMainWidget::OnFinishSkillEditing);
	}
	
	if(SaveSkillButton)
	{
		SaveSkillButton->OnClicked.AddDynamic(this, &USKSkillMakerTrainMainWidget::OnSaveSkillClicked);
	}

	if(BackButton)
	{
		BackButton->OnClicked.AddDynamic(this, &USKSkillMakerTrainMainWidget::OnBackClicked);
	}

	return true;
}

void USKSkillMakerTrainMainWidget::SetHUDReference(ASKSkillMakerTrainHUD* InHUD)
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));
	
	HUDReference = InHUD;

	if(SkillDetailWidget)
	{
		SkillDetailWidget->SetSkillMakerTrainHUD(HUDReference);
	}
}

void USKSkillMakerTrainMainWidget::SetSkillMakerState(ESKSkillMakerTrainState NewState, bool bFromBackNavigation)
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

	if (!bFromBackNavigation && CurrentState != NewState && NewState != ESKSkillMakerTrainState::ChooseAction)
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

void USKSkillMakerTrainMainWidget::GoBackToPreviousState()
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));
	
	if(PreviousStates.Num() <= 1)
	{
		SK_LOG(LogSkillMaker, Error, TEXT("이전 상태가 없으므로 뒤로 갈 수 없음."));
		return;
	}

	ESKSkillMakerTrainState PreviousState = PreviousStates.Last();
	PreviousStates.RemoveAt(PreviousStates.Num() - 1);

	SetSkillMakerState(PreviousState, true);
}

void USKSkillMakerTrainMainWidget::OnModifySkillClicked()
{
}

void USKSkillMakerTrainMainWidget::OnCreateSkillClicked()
{
}

void USKSkillMakerTrainMainWidget::OnSkillSelected(const FName& SkillID)
{
}

void USKSkillMakerTrainMainWidget::OnWeaponSelected(const FString& WeaponName)
{
}

void USKSkillMakerTrainMainWidget::OnAnimationSelected(UAnimMontage* AnimationMontage)
{
}

void USKSkillMakerTrainMainWidget::OnFinishSkillEditing()
{
}

void USKSkillMakerTrainMainWidget::OnSaveSkillClicked()
{
}

void USKSkillMakerTrainMainWidget::OnBackClicked()
{
}
