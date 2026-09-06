// Fill out your copyright notice in the Description page of Project Settings.


#include "SKSkillDetailWidget.h"
#include "UI/UI-SkillMaker/Common/ProjectileSelect/SKProjectileSelectionWidget.h"
#include "UI/UI-SkillMaker/Common/AnimationSelect/SKAnimNotifySelectionWidget.h"
#include "Components/ComboBoxString.h"
#include "Components/EditableTextBox.h"
#include "Components/Slider.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "UI/UI-SkillMaker/UEEditor/SKSkillMakerEditorHUD.h"
#include "UI/UI-SkillMaker/TrainingRoom/SKSkillMakerTrainHUD.h"
#include "UI/UI-SkillMaker/Common/StatusEffectSelect/SKStatusEffectCardWidget.h"
#include "Components/ScrollBox.h"
#include "GameFramework/Actor.h"
#include "Logging/SKLogSkillMakerMacro.h"

void USKSkillDetailWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (GeneralTabButton)
	{
		GeneralTabButton->OnClicked.AddDynamic(this, &USKSkillDetailWidget::OnGeneralTabClicked);
	}

	if (StatusEffectTabButton)
	{
		StatusEffectTabButton->OnClicked.AddDynamic(this, &USKSkillDetailWidget::OnStatusEffectTabClicked);
	}

	if (EffectSoundTabButton)
	{
		EffectSoundTabButton->OnClicked.AddDynamic(this, &USKSkillDetailWidget::OnEffectSoundTabClicked);
	}

	if (AnimNotifyTabButton)
	{
		AnimNotifyTabButton->OnClicked.AddDynamic(this, &USKSkillDetailWidget::OnAnimNotifyTabClicked);
	}

	if(SkillTypeComboBox)
	{
		SkillTypeComboBox->OnSelectionChanged.AddDynamic(this, &USKSkillDetailWidget::OnSkillTypeChanged);
	}

	/* Legacy: 프로토타입 세부사항 개편 전 데미지·범위 이벤트 바인딩
	DamageTextBox->OnTextCommitted.AddDynamic(this, &USKSkillDetailWidget::OnDamageChanged);
	MinRangeSlider->OnValueChanged.AddDynamic(this, &USKSkillDetailWidget::OnMinRangeChanged);
	MaxRangeSlider->OnValueChanged.AddDynamic(this, &USKSkillDetailWidget::OnMaxRangeChanged);
	*/

	if (PreviewSkillButton)
	{
		PreviewSkillButton->OnClicked.AddDynamic(this, &USKSkillDetailWidget::OnPreviewSkillClicked);
	}

	if (ProjectileSelectionWidget)
	{
		ProjectileSelectionWidget->OnProjectileSelected.AddDynamic(this, &USKSkillDetailWidget::OnProjectileSelected);
	}

	if (AnimNotifySelectionWidget)
	{
		AnimNotifySelectionWidget->OnAnimNotifySelected.AddDynamic(this, &USKSkillDetailWidget::OnNotifySelected);
	}

	PopulateStatusEffectList();
}

void USKSkillDetailWidget::SetSkillMakerEditorHUD(ASKSkillMakerEditorHUD* InHUD)
{
	if (SkillMakerEditorHUDReference)
	{
		OnSkillDetailChanged.RemoveDynamic(SkillMakerEditorHUDReference, &ASKSkillMakerEditorHUD::SetCurrentSkillData);
	}

	SkillMakerEditorHUDReference = InHUD;

	if(SkillMakerEditorHUDReference)
	{
		OnSkillDetailChanged.AddUniqueDynamic(SkillMakerEditorHUDReference, &ASKSkillMakerEditorHUD::SetCurrentSkillData);
		InitializeFromSkillData();
	}
}

void USKSkillDetailWidget::OnGeneralTabClicked()
{
	if (TabSwitcher)
	{
		TabSwitcher->SetActiveWidgetIndex(0);
	}
}

void USKSkillDetailWidget::OnStatusEffectTabClicked()
{
	if (TabSwitcher)
	{
		TabSwitcher->SetActiveWidgetIndex(1);
	}
}

void USKSkillDetailWidget::OnEffectSoundTabClicked()
{
	if (TabSwitcher)
	{
		TabSwitcher->SetActiveWidgetIndex(2);
	}
}

void USKSkillDetailWidget::OnAnimNotifyTabClicked()
{
	if (TabSwitcher)
	{
		TabSwitcher->SetActiveWidgetIndex(3);
		PopulateAnimNotifyList();
	}
}

void USKSkillDetailWidget::InitializeFromSkillData()
{
	if(!SkillMakerEditorHUDReference)
		return;

	PopularSkillDetails();
	PopulateStatusEffectList();

	if (ProjectileSelectionWidget)
	{
		ProjectileSelectionWidget->SetProjectileCard(SkillMakerEditorHUDReference->GetCurrentSkillData().ProjectileActor);
	}
}

void USKSkillDetailWidget::PopularSkillDetails()
{
	if (!SkillMakerEditorHUDReference)
		return;

	const FSKSkillData& SkillData = SkillMakerEditorHUDReference->GetCurrentSkillData();

	if(SkillTypeComboBox)
	{
		switch (SkillData.SkillType)
		{
		case ESkillType::Attack:
			SkillTypeComboBox->SetSelectedOption("Attack");
			break;
		case ESkillType::Buff:
			SkillTypeComboBox->SetSelectedOption("Buff");
			break;
		case ESkillType::Debuff:
			SkillTypeComboBox->SetSelectedOption("Debuff");
			break;
		}
	}

	/* Legacy: 프로토타입 세부사항 개편 전 데미지·범위 UI 초기화
	DamageTextBox->SetText(FText::AsNumber(SkillData.DamageValue));
	MinRangeSlider->SetValue(SkillData.MinRange);
	MaxRangeSlider->SetValue(SkillData.MaxRange);
	*/
}

void USKSkillDetailWidget::PopulateStatusEffectList()
{
	if (!StatusEffectListBox)
		return;

	StatusEffectListBox->ClearChildren();

	TMap<EStatusEffect, FStatusEffectData> ExistingEffects;
	if (SkillMakerEditorHUDReference)
	{
		for(const FStatusEffectData& Effect : SkillMakerEditorHUDReference->GetCurrentSkillData().StatusEffects)
		{
			ExistingEffects.Add(Effect.EffectType, Effect);
		}
	}

	for(EStatusEffect EffectType : TEnumRange<EStatusEffect>())
	{
		if(EffectType == EStatusEffect::None) continue;

		USKStatusEffectCardWidget* StatusEffectCard = CreateWidget<USKStatusEffectCardWidget>(this, WBP_SKStatusEffectCard);
		if(StatusEffectCard)
		{
			StatusEffectCard->InitializeEffectEntry(EffectType);

			if(ExistingEffects.Contains(EffectType))
			{
				StatusEffectCard->SetStatusEffectData(ExistingEffects[EffectType]);
			}

			StatusEffectCard->OnStatusEffectChanged.AddDynamic(this, &USKSkillDetailWidget::OnStatusEffectToggled);

			StatusEffectListBox->AddChild(StatusEffectCard);
		}
	}
}

void USKSkillDetailWidget::PopulateAnimNotifyList()
{
	if (!AnimNotifySelectionWidget || !SkillMakerEditorHUDReference) return;

	FSKSkillData SkillData = SkillMakerEditorHUDReference->GetCurrentSkillData();
	UAnimMontage* Montage = SkillData.SkillMontage.LoadSynchronous();
	if (Montage)
	{
		SkillData.SkillDuration = Montage->GetPlayLength();
		OnSkillDetailChanged.Broadcast(SkillData);
	}
	AnimNotifySelectionWidget->PopulateNotifyList(Montage, SkillData.NotifyName);
}

void USKSkillDetailWidget::SetSkillMakerTrainHUD(ASKSkillMakerTrainHUD* InHUD)
{
	SkillMakerTrainHUDReference = InHUD;

	if(SkillMakerTrainHUDReference)
	{
		InitializeFromSkillData();
	}
}

void USKSkillDetailWidget::OnSkillTypeChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (!SkillMakerEditorHUDReference)
		return;

	FSKSkillData SkillData = SkillMakerEditorHUDReference->GetCurrentSkillData();

	if (SelectedItem == "Attack")
	{
		SkillData.SkillType = ESkillType::Attack;
	}
	else if (SelectedItem == "Buff")
	{
		SkillData.SkillType = ESkillType::Buff;
	}
	else if (SelectedItem == "Debuff")
	{
		SkillData.SkillType = ESkillType::Debuff;
	}

	OnSkillDetailChanged.Broadcast(SkillData);
}

/* Legacy: 프로토타입 세부사항 개편 전 데미지·범위 변경 처리
void USKSkillDetailWidget::OnDamageChanged(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (!EditingSkillData.IsSet())
		return;

	EditingSkillData->DamageValue = FCString::Atof(*Text.ToString());
}
*/

void USKSkillDetailWidget::OnStatusEffectToggled(const FStatusEffectData& EffectData, bool bIsChecked)
{
	if (!SkillMakerEditorHUDReference)
		return;

	FSKSkillData SkillData = SkillMakerEditorHUDReference->GetCurrentSkillData();

	if (bIsChecked)
	{
		if (FStatusEffectData* ExistingEffect = SkillData.StatusEffects.FindByPredicate([&EffectData](const FStatusEffectData& Candidate)
		{
			return Candidate.EffectType == EffectData.EffectType;
		}))
		{
			*ExistingEffect = EffectData;
		}
		else
		{
			SkillData.StatusEffects.Add(EffectData);
		}
	}
	else
	{
		SkillData.StatusEffects.RemoveAll([&EffectData](const FStatusEffectData& Effect)
		{
			return Effect.EffectType == EffectData.EffectType;
		});
	}

	OnSkillDetailChanged.Broadcast(SkillData);
	SK_LOG(LogSkillMaker, Log, TEXT("상태 이상 업데이트됨. 총 개수: %d"), SkillData.StatusEffects.Num());
}

/* Legacy: 프로토타입 세부사항 개편 전 범위 변경 처리
void USKSkillDetailWidget::OnMinRangeChanged(float Value)
{
	if (!EditingSkillData.IsSet())
	return;

	EditingSkillData->MinRange = Value;
}

void USKSkillDetailWidget::OnMaxRangeChanged(float Value)
{
	if (!EditingSkillData.IsSet())
		return;

	EditingSkillData->MaxRange = Value;
}
*/

void USKSkillDetailWidget::OnProjectileSelected(TSoftClassPtr<ASKProjectileActor> SelectedProjectileClass)
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));

	if (!SkillMakerEditorHUDReference) return;

	FSKSkillData SkillData = SkillMakerEditorHUDReference->GetCurrentSkillData();
	SkillData.ProjectileActor = SelectedProjectileClass;
	OnSkillDetailChanged.Broadcast(SkillData);

	SK_LOG(LogSkillMaker, Log, TEXT("ProjectileSelected : %s"), *SelectedProjectileClass.ToSoftObjectPath().ToString());
}

void USKSkillDetailWidget::OnNotifySelected(FName NotifyName)
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));

	if (!SkillMakerEditorHUDReference) return;

	FSKSkillData SkillData = SkillMakerEditorHUDReference->GetCurrentSkillData();
	SkillData.NotifyName = NotifyName;
	OnSkillDetailChanged.Broadcast(SkillData);

	SK_LOG(LogSkillMaker, Log, TEXT("애님 노티파이 선택됨: %s"), *NotifyName.ToString());
}

void USKSkillDetailWidget::OnPreviewSkillClicked()
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));

	if (!SkillMakerEditorHUDReference)
	{
		SK_LOG(LogSkillMaker, Warning, TEXT("스킬 미리보기 불가: HUD 없음."));
		return;
	}

	SkillMakerEditorHUDReference->PreviewSkillEffect(SkillMakerEditorHUDReference->GetCurrentSkillData());
	SK_LOG(LogSkillMaker, Log, TEXT("스킬 미리보기 실행"));
}
