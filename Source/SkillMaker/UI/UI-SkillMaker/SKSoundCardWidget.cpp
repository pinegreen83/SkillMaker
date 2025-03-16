// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UI-SkillMaker/SKSoundCardWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

bool USKSoundCardWidget::Initialize()
{
	if (!Super::Initialize()) return false;

	if (SoundSelectButton)
	{
		SoundSelectButton->OnClicked.AddDynamic(this, &USKSoundCardWidget::OnSoundButtonClicked);
	}

	return true;
}

void USKSoundCardWidget::SetSoundInfo(USoundBase* InSound)
{
	Sound = InSound;

	if (SoundNameText && Sound)
	{
		SoundNameText->SetText(FText::FromString(Sound->GetName()));
	}
}

void USKSoundCardWidget::OnSoundButtonClicked()
{
	OnSoundSelected.Broadcast(Sound);
}
