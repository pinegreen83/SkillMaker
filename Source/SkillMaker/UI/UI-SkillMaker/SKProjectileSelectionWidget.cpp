// Fill out your copyright notice in the Description page of Project Settings.


#include "SKProjectileSelectionWidget.h"
#include "SKProjectileCardWidget.h"
#include "Components/ScrollBox.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Game/SKDataManagerSubsystem.h"
#include "Logging/SKLogSkillMakerMacro.h"

void USKProjectileSelectionWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (ConfirmButton)
	{
		ConfirmButton->OnClicked.AddDynamic(this, &USKProjectileSelectionWidget::OnConfirmSelection);
	}
}

bool USKProjectileSelectionWidget::Initialize()
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));
	
	bool Success = Super::Initialize();
	if (!Success)
		return false;
	
	SetProjectileCard();

	return true;
}

void USKProjectileSelectionWidget::SetProjectileCard()
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));
	
	if (!ProjectileListBox || !WBP_ProjectileCard) return;
	
	ProjectileListBox->ClearChildren();
	
	if(USKDataManagerSubsystem* DataManagerSubsystem = Cast<USKDataManagerSubsystem>(GetWorld()->GetGameInstance()))
	{
		TArray<FSKProjectileRow> ProjectileList = DataManagerSubsystem->GetProjectileList();
		for (auto& Projectile : ProjectileList)
		{
			USKProjectileCardWidget* ProjectileCard = CreateWidget<USKProjectileCardWidget>(this, WBP_ProjectileCard);
			if (!ProjectileCard) continue;
	
			ProjectileCard->SetProjectileInfo(Projectile.Data.ProjectileActor);
			ProjectileCard->OnProjectileCardSelected.AddDynamic(this, &USKProjectileSelectionWidget::SelectedProjectile);
	
			ProjectileListBox->AddChild(ProjectileCard);
		}
	}
}

void USKProjectileSelectionWidget::SelectedProjectile(TSubclassOf<ASKProjectileActor> SelectedProjectile)
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));
	
	SelectedProjectileData = SelectedProjectile;

	SK_LOG(LogSkillMaker, Log, TEXT("Now Selected Projectie : %s"), *SelectedProjectileData->GetName())
}

void USKProjectileSelectionWidget::OnConfirmSelection()
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));
	
	OnProjectileSelected.Broadcast(SelectedProjectileData);
}
