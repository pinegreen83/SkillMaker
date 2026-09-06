// Fill out your copyright notice in the Description page of Project Settings.


#include "SKProjectileSelectionWidget.h"
#include "SKProjectileCardWidget.h"
#include "Components/ScrollBox.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"
#include "Engine/World.h"
#include "Game/SKDataManagerSubsystem.h"
#include "Logging/SKLogSkillMakerMacro.h"

void USKProjectileSelectionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ConfirmButton)
	{
		ConfirmButton->SetVisibility(ESlateVisibility::Collapsed);
	}
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

void USKProjectileSelectionWidget::SetProjectileCard(const TSoftClassPtr<ASKProjectileActor>& CurrentProjectile,
	FGameplayTag CurrentElementTag)
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));

	UWorld* World = GetWorld();
	if (IsDesignTime() || !World || !World->IsGameWorld())
	{
		return;
	}
	
	if (!ProjectileListBox || !WBP_ProjectileCard)
	{
		SK_LOG(LogSkillMaker, Error, TEXT("ProjectileListBox 또는 WBP_ProjectileCard가 nullptr임."));
		return;
	}

	ProjectileListBox->ClearChildren();
	ProjectileCards.Reset();
	SelectedProjectileData = CurrentProjectile;
	if (SelectedEffectText)
	{
		SelectedEffectText->SetText(FText::FromString(CurrentProjectile.IsNull() ? TEXT("선택 없음") : CurrentProjectile.GetAssetName()));
	}

	bool bFoundCurrentProjectile = CurrentProjectile.IsNull();
	if (UGameInstance* GameInstance = World->GetGameInstance())
	{
		if (USKDataManagerSubsystem* DataManagerSubsystem = GameInstance->GetSubsystem<USKDataManagerSubsystem>())
		{
			TArray<FSKProjectileRow> ProjectileList = DataManagerSubsystem->GetProjectilesForElement(CurrentElementTag);
			for (const FSKProjectileRow& Projectile : ProjectileList)
			{
				USKProjectileCardWidget* ProjectileCard = CreateWidget<USKProjectileCardWidget>(this, WBP_ProjectileCard);
				if (!ProjectileCard) continue;

				const bool bIsCurrentProjectile = Projectile.Data.ProjectileActor == CurrentProjectile;
				bFoundCurrentProjectile |= bIsCurrentProjectile;
				ProjectileCard->SetProjectileInfo(Projectile.Data.ProjectileName, Projectile.Data.ProjectileActor, bIsCurrentProjectile);
				ProjectileCard->OnProjectileCardSelected.AddDynamic(this, &USKProjectileSelectionWidget::SelectedProjectile);

				ProjectileCards.Add(ProjectileCard);
				ProjectileListBox->AddChild(ProjectileCard);
			}
		}
		else
		{
			SK_LOG(LogSkillMaker, Error, TEXT("USKDataManagerSubsystem을 찾을 수 없음."));
		}
	}
	else
	{
		SK_LOG(LogSkillMaker, Error, TEXT("게임 월드에서 GameInstance를 찾을 수 없음."));
	}

	if (!bFoundCurrentProjectile)
	{
		SelectedProjectileData.Reset();
		if (SelectedEffectText)
		{
			SelectedEffectText->SetText(NSLOCTEXT("ProjectileSelection", "NoSelection", "선택 없음"));
		}
		SK_LOG(LogSkillMaker, Log, TEXT("선택한 속성과 호환되지 않는 기존 발사체 선택을 해제함."));
		OnProjectileSelected.Broadcast(SelectedProjectileData);
	}
}

void USKProjectileSelectionWidget::SelectedProjectile(TSoftClassPtr<ASKProjectileActor> SelectedProjectile)
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));
	
	SelectedProjectileData = SelectedProjectile;
	for (USKProjectileCardWidget* ProjectileCard : ProjectileCards)
	{
		if (ProjectileCard)
		{
			ProjectileCard->SetSelected(ProjectileCard->GetProjectileClass() == SelectedProjectileData);
		}
	}

	if (SelectedEffectText)
	{
		SelectedEffectText->SetText(FText::FromString(SelectedProjectileData.GetAssetName()));
	}

	SK_LOG(LogSkillMaker, Log, TEXT("Now Selected Projectile : %s"),
		SelectedProjectileData.IsNull() ? TEXT("None") : *SelectedProjectileData.ToSoftObjectPath().ToString());

	OnProjectileSelected.Broadcast(SelectedProjectileData);
}
