// Fill out your copyright notice in the Description page of Project Settings.


#include "SKWeaponSelectionWidget.h"
#include "Components/UniformGridPanel.h"
#include "Data/SKDataManager.h"
#include "Data/SKWeaponData.h"
#include "SKWeaponCardWidget.h"
#include "Logging/SKLogSkillMakerMacro.h"

void USKWeaponSelectionWidget::NativeConstruct()
{
	Super::NativeConstruct();
	LoadWeaponList();
}

void USKWeaponSelectionWidget::LoadWeaponList()
{
	SK_LOG(LogSkillMaker, Log, TEXT("무기 목록을 불러옵니다."));

	TArray<FSKWeaponRow> WeaponList = USKDataManager::Get()->GetWeaponList();

	int32 index = 0;
	for(const auto& WeaponData : WeaponList)
	{
		SK_LOG(LogSkillMaker, Log, TEXT("무기 로드 : %s"), *WeaponData.RowName.ToString());
		
		CreateWeaponCard(WeaponData, index);
		index++;
	}
}

void USKWeaponSelectionWidget::CreateWeaponCard(const FSKWeaponRow& WeaponRow, const int32 WeaponIndex)
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));
	
	if(!WeaponGridPanel || !WBP_SKWeaponCard)
		return;

	if(USKWeaponCardWidget* WeaponCard = CreateWidget<USKWeaponCardWidget>(GetWorld(), WBP_SKWeaponCard))
	{
		const FString WeaponName = WeaponRow.Data.WeaponName;
		const FString WeaponType = WeaponRow.Data.WeaponType;
		UTexture2D* Thumbnail = WeaponRow.Data.WeaponThumbnail.LoadSynchronous();
		
		WeaponCard->SetWeaponInfo(WeaponName, WeaponType, Thumbnail);
		WeaponCard->OnWeaponCardSelected.AddDynamic(this, &USKWeaponSelectionWidget::WeaponSelected);
		WeaponGridPanel->AddChildToUniformGrid(WeaponCard, WeaponIndex/2, WeaponIndex%2);
	}
}

void USKWeaponSelectionWidget::WeaponSelected(const FString& WeaponName)
{
	SK_LOG(LogSkillMaker, Log, TEXT("선택된 무기 : %s"), *WeaponName);

	OnWeaponSelected.Broadcast(WeaponName);
}

