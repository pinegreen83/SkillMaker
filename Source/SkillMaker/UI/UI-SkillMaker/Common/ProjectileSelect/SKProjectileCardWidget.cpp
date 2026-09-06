// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UI-SkillMaker/Common/ProjectileSelect/SKProjectileCardWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Logging/SKLogSkillMakerMacro.h"

bool USKProjectileCardWidget::Initialize()
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));
	
	if (!Super::Initialize()) return false;

	if (ProjectileSelectButton)
	{
		ProjectileSelectButton->OnClicked.AddDynamic(this, &USKProjectileCardWidget::OnProjectileButtonClicked);
	}

	return true;
}

void USKProjectileCardWidget::SetProjectileInfo(const FString& ProjectileName, const TSoftClassPtr<ASKProjectileActor>& InProjectileClass, bool bIsSelected)
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));
	
	ProjectileClass = InProjectileClass;

	if (ProjectileNameText)
	{
		const FString DisplayName = ProjectileName.IsEmpty() ? ProjectileClass.GetAssetName() : ProjectileName;
		ProjectileNameText->SetText(FText::FromString(DisplayName));
	}

	if (ProjectileSelectButton)
	{
		ProjectileSelectButton->SetBackgroundColor(bIsSelected ? FLinearColor(0.15f, 0.55f, 1.0f, 1.0f) : FLinearColor::White);
	}
}

void USKProjectileCardWidget::OnProjectileButtonClicked()
{
	if (ProjectileClass.IsNull())
	{
		return;
	}

	OnProjectileCardSelected.Broadcast(ProjectileClass);
}
