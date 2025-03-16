// Fill out your copyright notice in the Description page of Project Settings.


#include "SKEffectSoundSelectionWidget.h"
#include "SKEffectCardWidget.h"
#include "SKSoundCardWidget.h"
#include "Components/ScrollBox.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Components/WidgetSwitcher.h"

void USKEffectSoundSelectionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (EffectTabButton)
	{
		EffectTabButton->OnClicked.AddDynamic(this, &USKEffectSoundSelectionWidget::OnEffectTabClicked);
	}

	if (SoundTabButton)
	{
		SoundTabButton->OnClicked.AddDynamic(this, &USKEffectSoundSelectionWidget::OnSoundTabClicked);
	}

	if (FinalTabButton)
	{
		FinalTabButton->OnClicked.AddDynamic(this, &USKEffectSoundSelectionWidget::OnFinalTabClicked);
	}
}

bool USKEffectSoundSelectionWidget::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success)
		return false;

	if (ConfirmButton)
	{
		ConfirmButton->OnClicked.AddDynamic(this, &USKEffectSoundSelectionWidget::OnConfirmSelection);
	}

	PopulateEffectList();
	PopulateSoundList();

	return true;
}

void USKEffectSoundSelectionWidget::PopulateEffectList()
{
	if (!EffectListBox || !WBP_EffectCard) return;
	
	EffectListBox->ClearChildren();
	
	FAssetRegistryModule& AssetRegistry = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	TArray<FAssetData> EffectAssets;
	FTopLevelAssetPath EffectAssetPath(TEXT("/Script/Engine.Actor"));
	AssetRegistry.Get().GetAssetsByClass(EffectAssetPath, EffectAssets);
	
	for (const FAssetData& AssetData : EffectAssets)
	{
		TSubclassOf<AActor> EffectClass = Cast<UClass>(AssetData.GetAsset());
		if (!EffectClass) continue;
	
		USKEffectCardWidget* EffectCard = CreateWidget<USKEffectCardWidget>(this, WBP_EffectCard);
		if (!EffectCard) continue;
	
		EffectCard->SetEffectInfo(EffectClass);
		EffectCard->OnEffectSelected.AddDynamic(this, &USKEffectSoundSelectionWidget::OnEffectSelected);
	
		EffectListBox->AddChild(EffectCard);
	}
}

void USKEffectSoundSelectionWidget::PopulateSoundList()
{
	if (!SoundListBox || !WBP_SoundCard) return;
	
	SoundListBox->ClearChildren();
	
	FAssetRegistryModule& AssetRegistry = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	TArray<FAssetData> SoundAssets;
	FTopLevelAssetPath SoundAssetPath(TEXT("/Script/Engine.SoundBase"));
	AssetRegistry.Get().GetAssetsByClass(SoundAssetPath, SoundAssets);
	
	for (const FAssetData& AssetData : SoundAssets)
	{
		USoundBase* Sound = Cast<USoundBase>(AssetData.GetAsset());
		if (!Sound) continue;
	
		USKSoundCardWidget* SoundCard = CreateWidget<USKSoundCardWidget>(this, WBP_SoundCard);
		if (!SoundCard) continue;
	
		SoundCard->SetSoundInfo(Sound);
		SoundCard->OnSoundSelected.AddDynamic(this, &USKEffectSoundSelectionWidget::OnSoundSelected);
	
		SoundListBox->AddChild(SoundCard);
	}
}

void USKEffectSoundSelectionWidget::OnEffectTabClicked()
{
	if (TabSwitcher)
	{
		TabSwitcher->SetActiveWidgetIndex(0);
	}
}

void USKEffectSoundSelectionWidget::OnSoundTabClicked()
{
	if (TabSwitcher)
	{
		TabSwitcher->SetActiveWidgetIndex(1);
	}
}

void USKEffectSoundSelectionWidget::OnFinalTabClicked()
{
	if (TabSwitcher)
	{
		TabSwitcher->SetActiveWidgetIndex(2);
	}
}

void USKEffectSoundSelectionWidget::OnEffectSelected(TSubclassOf<AActor> SelectedEffect)
{
	SelectedEffectSoundData.EffectClass = SelectedEffect;

	/** 선택된 이펙트 미리보기 업데이트 (향후 썸네일 이미지 추가 가능) */
	if (SelectedEffectPreview)
	{
		SelectedEffectPreview->SetVisibility(ESlateVisibility::Visible);
	}
}

void USKEffectSoundSelectionWidget::OnSoundSelected(USoundBase* SelectedSound)
{
	SelectedEffectSoundData.Sound = SelectedSound;

	/** 선택된 사운드 이름 업데이트 */
	if (SelectedSoundText)
	{
		SelectedSoundText->SetText(FText::FromString(SelectedSound->GetName()));
	}
}

void USKEffectSoundSelectionWidget::OnConfirmSelection()
{
	OnEffectSoundSelected.Broadcast(SelectedEffectSoundData);
}