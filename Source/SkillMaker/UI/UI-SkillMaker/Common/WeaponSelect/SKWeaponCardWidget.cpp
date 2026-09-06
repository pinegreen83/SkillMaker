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

void USKWeaponCardWidget::SetWeaponInfo(const FString& InWeaponName, FGameplayTag InWeaponTag, const TSoftObjectPtr<UTexture2D>& InThumbnail, bool bIsSelected)
{
	if(WeaponNameText)
	{
		WeaponNameText->SetText(FText::FromString(InWeaponName));
	}

	if(WeaponThumbnail)
	{
		WeaponThumbnail->SetBrushFromSoftTexture(InThumbnail);
	}

	WeaponTag = InWeaponTag;
	if (WeaponButton)
	{
		WeaponButton->SetBackgroundColor(bIsSelected ? FLinearColor(0.15f, 0.55f, 1.0f, 1.0f) : FLinearColor::White);
	}
}

void USKWeaponCardWidget::HandleWeaponSelected()
{
	if (!WeaponTag.IsValid())
	{
		SK_LOG(LogSkillMaker, Warning, TEXT("무기 카드에 유효한 태그가 설정되지 않음."));
		return;
	}

	OnWeaponCardSelected.Broadcast(WeaponTag);
}
