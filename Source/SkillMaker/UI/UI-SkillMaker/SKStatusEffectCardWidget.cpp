// Fill out your copyright notice in the Description page of Project Settings.


#include "SKStatusEffectCardWidget.h"
#include "Components/CheckBox.h"
#include "Components/EditableTextBox.h"
#include "Components/Slider.h"

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

	return true;
}

void USKStatusEffectCardWidget::InitializeEffectEntry(EStatusEffect EffectType)
{
	CurrentEffectType = EffectType;

	if (EffectCheckBox)
	{
		EffectCheckBox->SetIsChecked(false);
	}

	if (DurationTextBox)
	{
		DurationTextBox->SetText(FText::AsNumber(0.0f));
	}

	if (DOTSlider)
	{
		DOTSlider->SetValue(0.0f);
	}
}

void USKStatusEffectCardWidget::OnEffectToggled(bool bIsChecked)
{
	float Duration = FCString::Atof(*DurationTextBox->GetText().ToString());
	float DOT = DOTSlider->GetValue();
	OnStatusEffectToggled.Broadcast(CurrentEffectType, bIsChecked, Duration, DOT);
}

void USKStatusEffectCardWidget::OnDurationChanged(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (EffectCheckBox && EffectCheckBox->IsChecked())
	{
		OnEffectToggled(true);
	}
}

void USKStatusEffectCardWidget::OnDOTValueChanged(float Value)
{
	if (EffectCheckBox && EffectCheckBox->IsChecked())
	{
		OnEffectToggled(true);
	}
}