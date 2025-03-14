// Fill out your copyright notice in the Description page of Project Settings.


#include "SKSkillMakerMainWidget.h"
#include "SKSkillSelectionWidget.h"
#include "SKAnimationSelectionWidget.h"
#include "SKWeaponSelectionWidget.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "SKSkillMakerHUD.h"

bool USKSkillMakerMainWidget::Initialize()
{
	bool Success = Super::Initialize();
	if(!Success)
		return false;

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
	HUDReference = InHUD;
}

void USKSkillMakerMainWidget::SetSkillMakerState(ESKSkillMakerState NewState)
{
	if(!SkillMakerSwitcher)
	{
		UE_LOG(LogTemp, Error, TEXT("WidgetSwitcher가 nullptr"));
		return;
	}

	if (static_cast<int32>(NewState) >= SkillMakerSwitcher->GetNumWidgets())
	{
		UE_LOG(LogTemp, Error, TEXT("❌ 잘못된 WidgetSwitcher 인덱스! NewState: %d, 총 위젯 개수: %d"),
			   static_cast<int32>(NewState), SkillMakerSwitcher->GetNumWidgets());
		return;
	}

	if(CurrentState != NewState)
	{
		PreviousStates.Add(CurrentState);
	}

	CurrentState = NewState;
	SkillMakerSwitcher->SetActiveWidgetIndex(static_cast<int32>(NewState));

	if(BackButton)
	{
		BackButton->SetVisibility(PreviousStates.Num() > 0 ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
}

void USKSkillMakerMainWidget::GoBackToPreviousState()
{
	if(PreviousStates.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("이전 상태가 없으므로 뒤로 갈 수 없음."));
		return;
	}

	ESKSkillMakerState PreviousState = PreviousStates.Last();
	PreviousStates.RemoveAt(PreviousStates.Num() - 1);

	SetSkillMakerState(PreviousState);
}

void USKSkillMakerMainWidget::OnModifySkillClicked()
{
	UE_LOG(LogTemp, Log, TEXT("기존 스킬 수정 시작"));
	SetSkillMakerState(ESKSkillMakerState::ChooseSkill);
}

void USKSkillMakerMainWidget::OnCreateSkillClicked()
{
	UE_LOG(LogTemp, Log, TEXT("새로운 스킬 생성 시작"));
	SetSkillMakerState(ESKSkillMakerState::ChooseWeapon);
}

void USKSkillMakerMainWidget::OnSkillSelected(const FName& SkillID)
{
	if(!HUDReference)
	{
		UE_LOG(LogTemp, Error, TEXT("HUD 레퍼런스 없음."));
		return;
	}
	
	HUDReference->LoadSkillForEditing(SkillID);
	UE_LOG(LogTemp, Log, TEXT("스킬 선택됨 : %s"), *SkillID.ToString());
	SetSkillMakerState(ESKSkillMakerState::ChooseWeapon);
}

void USKSkillMakerMainWidget::OnWeaponSelected(const FString& WeaponType)
{
	if(!HUDReference)
	{
		UE_LOG(LogTemp, Error, TEXT("HUD 레퍼런스 없음."));
		return;
	}
	
	SelectedWeaponType = WeaponType;
	UE_LOG(LogTemp, Log, TEXT("선택된 무기 : %s"), *SelectedWeaponType);
	
	HUDReference->SetSkillWeaponType(WeaponType);
	
	if(AnimationSelectionWidget)
	{
		AnimationSelectionWidget->LoadAnimationsForWeapon(WeaponType);
	}

	SetSkillMakerState(ESKSkillMakerState::ChooseAnimation);
}

void USKSkillMakerMainWidget::OnAnimationSelected(UAnimMontage* AnimationMontage)
{
	if(!HUDReference)
	{
		UE_LOG(LogTemp, Error, TEXT("HUD 레퍼런스 없음."));
		return;
	}

	if(AnimationMontage)
	{
		HUDReference->SetSkillMontage(AnimationMontage);
		UE_LOG(LogTemp, Log, TEXT("선택된 애니메이션 : %s"), *AnimationMontage->GetName());

		SetSkillMakerState(ESKSkillMakerState::EditSkill);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("애니메이션이 선택되지 않음."));
	}
}

void USKSkillMakerMainWidget::OnFinishSkillEditing()
{
	SetSkillMakerState(ESKSkillMakerState::SaveSkill);
}

void USKSkillMakerMainWidget::OnSaveSkillClicked()
{
	if(!HUDReference)
	{
		UE_LOG(LogTemp, Error, TEXT("HUD 레퍼런스 없음."));
		return;
	}
	
	FString SkillName = SkillNameInput->GetText().ToString();
	if(SkillName.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("스킬 이름이 입력되지 않음."));
		return;
	}

	HUDReference->SetSkillName(SkillName);
	UE_LOG(LogTemp, Log, TEXT("스킬 %s 저장 완료"), *SkillName);
	HUDReference->LogCurrentSkillData();
	
	SetSkillMakerState(ESKSkillMakerState::ChooseAction);
}

void USKSkillMakerMainWidget::OnBackClicked()
{
	GoBackToPreviousState();
}
