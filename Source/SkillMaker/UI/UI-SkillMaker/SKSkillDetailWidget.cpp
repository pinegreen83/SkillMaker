// Fill out your copyright notice in the Description page of Project Settings.


#include "SKSkillDetailWidget.h"
#include "SKEffectSoundSelectionWidget.h"
#include "SKAnimNotifySelectionWidget.h"
#include "Components/ComboBoxString.h"
#include "Components/EditableTextBox.h"
#include "Components/Slider.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "SKSkillMakerHUD.h"
#include "SKStatusEffectCardWidget.h"
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

	if(DamageTextBox)
	{
		DamageTextBox->OnTextCommitted.AddDynamic(this, &USKSkillDetailWidget::OnDamageChanged);
	}

	if(MinRangeSlider)
	{
		MinRangeSlider->OnValueChanged.AddDynamic(this, &USKSkillDetailWidget::OnMinRangeChanged);
	}

	if(MaxRangeSlider)
	{
		MaxRangeSlider->OnValueChanged.AddDynamic(this, &USKSkillDetailWidget::OnMaxRangeChanged);
	}
	
	if (PreviewSkillButton)
	{
		PreviewSkillButton->OnClicked.AddDynamic(this, &USKSkillDetailWidget::OnPreviewSkillClicked);
	}
	
	// if(SaveSkillButton)
	// {
	// 	SaveSkillButton->OnClicked.AddDynamic(this, &USKSkillDetailWidget::OnSaveSkillClicked);
	// }

	if (EffectSoundSelectionWidget)
	{
		EffectSoundSelectionWidget->OnEffectSoundSelected.AddDynamic(this, &USKSkillDetailWidget::OnEffectSoundSelected);
	}

	if (AnimNotifySelectionWidget)
	{
		AnimNotifySelectionWidget->OnAnimNotifySelected.AddDynamic(this, &USKSkillDetailWidget::OnNotifySelected);
	}
	
	PopulateStatusEffectList();
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

void USKSkillDetailWidget::SetHUDReference(ASKSkillMakerHUD* InHUD)
{
	HUDReference = InHUD;

	if(HUDReference)
	{
		InitializeFromSkillData();
	}
}

void USKSkillDetailWidget::InitializeFromSkillData()
{
	if(!HUDReference)
			return;

	EditingSkillData = HUDReference->GetCurrentSkillData();

	if(!EditingSkillData.IsSet())
		return;

	PopularSkillDetails();
}

void USKSkillDetailWidget::PopularSkillDetails()
{
	if(!EditingSkillData.IsSet())
		return;

	FSKSkillData& SkillData = EditingSkillData.GetValue();

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

	if(DamageTextBox)
	{
		DamageTextBox->SetText(FText::AsNumber(SkillData.DamageValue));
	}

	if (MinRangeSlider)
	{
		MinRangeSlider->SetValue(SkillData.MinRange);
	}

	if (MaxRangeSlider)
	{
		MaxRangeSlider->SetValue(SkillData.MaxRange);
	}
}

void USKSkillDetailWidget::PopulateStatusEffectList()
{
	if (!StatusEffectListBox)
		return;

	StatusEffectListBox->ClearChildren();
	
	TMap<EStatusEffect, FStatusEffectData> ExistingEffects;
	if(EditingSkillData.IsSet())
	{
		for(const FStatusEffectData& Effect : EditingSkillData->StatusEffects)
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

			StatusEffectListBox->AddChild(StatusEffectCard);
		}
	}
}

void USKSkillDetailWidget::PopulateAnimNotifyList()
{
	if (!AnimNotifySelectionWidget || !EditingSkillData.IsSet()) return;
	
	AnimNotifySelectionWidget->PopulateNotifyList(EditingSkillData->SkillMontage);
}

void USKSkillDetailWidget::SaveSkillData()
{
	if(!EditingSkillData.IsSet() || !HUDReference)
		return;

	SaveSkillDetails();

	HUDReference->SetCurrentSkillData(EditingSkillData.GetValue());

	SK_LOG(LogSkillMaker, Log, TEXT("스킬 데이터 저장 완료."));
}

void USKSkillDetailWidget::SaveSkillDetails()
{
	if(!EditingSkillData.IsSet())
		return;

	FSKSkillData& SkillData = EditingSkillData.GetValue();

	if(SkillTypeComboBox)
	{
		FString SelectedOption = SkillTypeComboBox->GetSelectedOption();
		if (SelectedOption == "Attack")
		{
			SkillData.SkillType = ESkillType::Attack;
		}
		else if (SelectedOption == "Buff")
		{
			SkillData.SkillType = ESkillType::Buff;
		}
		else if (SelectedOption == "Debuff")
		{
			SkillData.SkillType = ESkillType::Debuff;
		}
	}

	if(DamageTextBox)
	{
		SkillData.DamageValue = FCString::Atof(*DamageTextBox->GetText().ToString());
	}

	if(MinRangeSlider)
	{
		SkillData.MinRange = MinRangeSlider->GetValue();
	}

	if(MaxRangeSlider)
	{
		SkillData.MaxRange = MaxRangeSlider->GetValue();
	}
}

void USKSkillDetailWidget::OnSkillTypeChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if(!EditingSkillData.IsSet())
		return;
	
	if (SelectedItem == "Attack")
	{
		EditingSkillData->SkillType = ESkillType::Attack;
	}
	else if (SelectedItem == "Buff")
	{
		EditingSkillData->SkillType = ESkillType::Buff;
	}
	else if (SelectedItem == "Debuff")
	{
		EditingSkillData->SkillType = ESkillType::Debuff;
	}
}

void USKSkillDetailWidget::OnDamageChanged(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (!EditingSkillData.IsSet())
		return;

	EditingSkillData->DamageValue = FCString::Atof(*Text.ToString());
}

void USKSkillDetailWidget::OnStatusEffectToggled(EStatusEffect EffectType, bool bIsChecked, float Duration,
	float DamageOverTime)
{
	if (!EditingSkillData.IsSet())
		return;

	FSKSkillData& SkillData = EditingSkillData.GetValue();

	if (bIsChecked)
	{
		for (const FStatusEffectData& ExistingEffect : SkillData.StatusEffects)
		{
			if (ExistingEffect.EffectType == EffectType)
			{
				return;
			}
		}

		SkillData.StatusEffects.Add(FStatusEffectData(EffectType, Duration, DamageOverTime));
	}
	else
	{
		SkillData.StatusEffects.RemoveAll([EffectType](const FStatusEffectData& Effect)
		{
			return Effect.EffectType == EffectType;
		});
	}

	SK_LOG(LogSkillMaker, Log, TEXT("상태 이상 업데이트됨. 총 개수: %d"), SkillData.StatusEffects.Num());
}

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

void USKSkillDetailWidget::OnEffectSoundSelected(const FEffectSoundData& SelectedEffectSound)
{
	if (!EditingSkillData.IsSet()) return;

	EditingSkillData->EffectSoundData = SelectedEffectSound;

	SK_LOG(LogSkillMaker, Log, TEXT("이펙트 및 사운드 추가됨: %s (이펙트: %s, 사운드: %s)"), 
		*SelectedEffectSound.NotifyName.ToString(),
		SelectedEffectSound.EffectClass ? *SelectedEffectSound.EffectClass->GetName() : TEXT("None"),
		SelectedEffectSound.Sound ? *SelectedEffectSound.Sound->GetName() : TEXT("None"));
}

void USKSkillDetailWidget::OnNotifySelected(FName NotifyName, float NotifyTime)
{
	if (!EditingSkillData.IsSet()) return;

	EditingSkillData->EffectSoundData.NotifyName = NotifyName;
	EditingSkillData->EffectSoundData.NotifyTime = NotifyTime;
	
	SK_LOG(LogSkillMaker, Log, TEXT("애님 노티파이 선택됨: %s"), *NotifyName.ToString());
}

void USKSkillDetailWidget::OnPreviewSkillClicked()
{
	if (!HUDReference || !EditingSkillData.IsSet()) return;
	
	HUDReference->PreviewSkillEffect(EditingSkillData.GetValue());
	SK_LOG(LogSkillMaker, Log, TEXT("스킬 미리보기 실행"));
}

void USKSkillDetailWidget::OnSaveSkillClicked()
{
	SaveSkillData();
}
