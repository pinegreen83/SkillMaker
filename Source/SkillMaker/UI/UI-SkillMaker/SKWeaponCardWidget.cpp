// Fill out your copyright notice in the Description page of Project Settings.


#include "SKWeaponCardWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Logging/SKLogSkillMakerMacro.h"

bool USKWeaponCardWidget::Initialize()
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));
	
	bool Success = Super::Initialize();
	if(!Success)
		return false;

	if(WeaponButton)
	{
		WeaponButton->OnClicked.AddDynamic(this, &USKWeaponCardWidget::HandleWeaponSelected);
	}

	return true;
}

void USKWeaponCardWidget::SetWeaponInfo(const FString& InWeaponName, const FString& InWeaponType, UTexture2D* InThumbnail)
{
	if(WeaponNameText)
	{
		WeaponNameText->SetText(FText::FromString(InWeaponName));
	}

	if(WeaponThumbnail)
	{
		WeaponThumbnail->SetBrushFromTexture(InThumbnail);
	}

	WeaponType = InWeaponType;
}

void USKWeaponCardWidget::HandleWeaponSelected()
{
	OnWeaponCardSelected.Broadcast(WeaponType);
}
