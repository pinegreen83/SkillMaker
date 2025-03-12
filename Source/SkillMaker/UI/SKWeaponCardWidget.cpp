// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SKWeaponCardWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

bool USKWeaponCardWidget::Initialize()
{
	bool Success = Super::Initialize();
	if(!Success) return false;

	if(WeaponButton)
	{
		WeaponButton->OnClicked.AddDynamic(this, &USKWeaponCardWidget::HandleWeaponSelected);
	}

	return true;
}

void USKWeaponCardWidget::SetWeaponInfo(const FString& WeaponName, UTexture2D* Thumbnail, const FString& InWeaponType)
{
	if(WeaponNameText)
	{
		WeaponNameText->SetText(FText::FromString(WeaponName));
	}

	if(WeaponThumbnail)
	{
		WeaponThumbnail->SetBrushFromTexture(Thumbnail);
	}

	WeaponType = InWeaponType;
}

void USKWeaponCardWidget::HandleWeaponSelected()
{
	OnWeaponCardSelected.Broadcast(WeaponType);
}
