// Fill out your copyright notice in the Description page of Project Settings.


#include "SKStatusEffectCardWidget.h"
#include "Components/CheckBox.h"
#include "Components/EditableTextBox.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"

bool USKStatusEffectCardWidget::Initialize()
{
	Super::Initialize();

	if (EffectCheckBox)
	{
		EffectCheckBox->OnCheckStateChanged.AddDynamic(this, &USKStatusEffectCardWidget::OnEffectToggled);
	}

	if (DurationTextBox)
	{
		DurationTextBox->OnTextCommitted.AddDynamic(this, &USKStatusEffectCardWidget::OnDurationChanged);
	}

	if (DOTSlider)
	{
		DOTSlider->OnValueChanged.AddDynamic(this, &USKStatusEffectCardWidget::OnDOTValueChanged);
	}

	if(StackCountTextBox)
	{
		StackCountTextBox->OnTextCommitted.AddDynamic(this, &USKStatusEffectCardWidget::OnStackCountChanged);
	}
	
	return true;
}

void USKStatusEffectCardWidget::InitializeEffectEntry(EStatusEffect EffectType)
{
	CurrentEffectType = EffectType;
	CurrentDuration = 0.0f;
	CurrentDOT = 0.0f;
	CurrentStackCount = 1;

	if(EffectNameText)
	{
		FString EffectName;
		switch(EffectType)
		{
		case EStatusEffect::Stun:	EffectName = "Stun"; break;
		case EStatusEffect::Slow:	EffectName = "Slow"; break;
		case EStatusEffect::Burn:	EffectName = "Burn"; break;
		case EStatusEffect::Freeze:	EffectName = "Freeze"; break;
		case EStatusEffect::Poison:	EffectName = "Poison"; break;
		case EStatusEffect::Shock:	EffectName = "Shock"; break;
		case EStatusEffect::Curse:	EffectName = "Curse"; break;
		default:										break;
		}
		EffectNameText->SetText(FText::FromString(EffectName));
	}
	
	if(EffectCheckBox)
	{
		EffectCheckBox->SetIsChecked(false);
	}

	if(DurationTextBox)
	{
		DurationTextBox->SetText(FText::AsNumber(0.0f));
	}

	if(DOTSlider)
	{
		DOTSlider->SetValue(0.0f);
	}

	if(StackCountTextBox)
	{
		StackCountTextBox->SetText(FText::AsNumber(1));
	}
}

void USKStatusEffectCardWidget::SetStatusEffectData(const FStatusEffectData& EffectData)
{
	CurrentDuration = EffectData.Duration;
	CurrentDOT = EffectData.DamagePerSecond;
	CurrentStackCount = EffectData.MaxStack;

	if(EffectCheckBox)
	{
		EffectCheckBox->SetIsChecked(true);
	}

	if(DurationTextBox)
	{
		DurationTextBox->SetText(FText::AsNumber(EffectData.Duration));
	}

	if(DOTSlider)
	{
		DOTSlider->SetValue(EffectData.DamagePerSecond);
	}

	if(StackCountTextBox)
	{
		StackCountTextBox->SetText(FText::AsNumber(EffectData.MaxStack));
	}
}

FStatusEffectData USKStatusEffectCardWidget::GetCurrentStatusEffectData() const
{
	FStatusEffectData EffectData;
	EffectData.EffectType = CurrentEffectType;
	EffectData.Duration = CurrentDuration;
	EffectData.DamagePerSecond = CurrentDOT;
	EffectData.MaxStack = CurrentStackCount;

	return EffectData;
}

void USKStatusEffectCardWidget::OnEffectToggled(bool bIsChecked)
{
	if (!bIsChecked)
	{
		CurrentDuration = 0.0f;
		CurrentDOT = 0.0f;
		CurrentStackCount = 1;
	}
}

void USKStatusEffectCardWidget::OnDurationChanged(const FText& Text, ETextCommit::Type CommitMethod)
{
	CurrentDuration = FCString::Atof(*Text.ToString());
}

void USKStatusEffectCardWidget::OnDOTValueChanged(float Value)
{
	CurrentDOT = Value;
}

void USKStatusEffectCardWidget::OnStackCountChanged(const FText& Text, ETextCommit::Type CommitMethod)
{
	CurrentStackCount = FCString::Atoi(*Text.ToString());
}
