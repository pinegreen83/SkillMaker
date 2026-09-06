// Fill out your copyright notice in the Description page of Project Settings.


#include "SKProjectileSelectionWidget.h"
#include "SKProjectileCardWidget.h"
#include "Components/ScrollBox.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "Game/SKDataManagerSubsystem.h"
#include "Logging/SKLogSkillMakerMacro.h"

void USKProjectileSelectionWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
}

bool USKProjectileSelectionWidget::Initialize()
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));
	
	bool Success = Super::Initialize();
	if (!Success)
		return false;
	
	SetProjectileCard(TSoftClassPtr<ASKProjectileActor>());

	return true;
}

void USKProjectileSelectionWidget::SetProjectileCard(const TSoftClassPtr<ASKProjectileActor>& CurrentProjectile)
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));
	
	if (!ProjectileListBox || !WBP_ProjectileCard)
	{
		SK_LOG(LogSkillMaker, Error, TEXT("ProjectileListBox 또는 WBP_ProjectileCard가 nullptr임."));
		return;
	}

	ProjectileListBox->ClearChildren();
	SelectedProjectileData = CurrentProjectile;
	if (SelectedEffectText)
	{
		SelectedEffectText->SetText(FText::FromString(CurrentProjectile.IsNull() ? TEXT("선택 없음") : CurrentProjectile.GetAssetName()));
	}

	if(USKDataManagerSubsystem* DataManagerSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<USKDataManagerSubsystem>())
	{
		TArray<FSKProjectileRow> ProjectileList = DataManagerSubsystem->GetProjectileList();
		for (auto& Projectile : ProjectileList)
		{
			USKProjectileCardWidget* ProjectileCard = CreateWidget<USKProjectileCardWidget>(this, WBP_ProjectileCard);
			if (!ProjectileCard) continue;
	
			ProjectileCard->SetProjectileInfo(Projectile.Data.ProjectileName, Projectile.Data.ProjectileActor,
				Projectile.Data.ProjectileActor == CurrentProjectile);
			ProjectileCard->OnProjectileCardSelected.AddDynamic(this, &USKProjectileSelectionWidget::SelectedProjectile);
	
			ProjectileListBox->AddChild(ProjectileCard);
		}
	}
	else
	{
		SK_LOG(LogSkillMaker, Error, TEXT("USKDataManagerSubsystem을 찾을 수 없음."));
	}
}

void USKProjectileSelectionWidget::SelectedProjectile(TSoftClassPtr<ASKProjectileActor> SelectedProjectile)
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));
	
	SelectedProjectileData = SelectedProjectile;
	if (SelectedEffectText)
	{
		SelectedEffectText->SetText(FText::FromString(SelectedProjectileData.GetAssetName()));
	}

	SK_LOG(LogSkillMaker, Log, TEXT("Now Selected Projectile : %s"),
		SelectedProjectileData.IsNull() ? TEXT("None") : *SelectedProjectileData.ToSoftObjectPath().ToString());

	OnProjectileSelected.Broadcast(SelectedProjectileData);
}
