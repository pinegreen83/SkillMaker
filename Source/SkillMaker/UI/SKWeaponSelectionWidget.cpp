// Fill out your copyright notice in the Description page of Project Settings.


#include "SKWeaponSelectionWidget.h"
#include "Components/UniformGridPanel.h"
#include "Data/SKDataManager.h"
#include "Data/SKWeaponData.h"
#include "SKWeaponCardWidget.h"

void USKWeaponSelectionWidget::NativeConstruct()
{
	Super::NativeConstruct();
	LoadWeaponList();
}

void USKWeaponSelectionWidget::LoadWeaponList()
{
	UE_LOG(LogTemp, Log, TEXT("무기 목록을 불러옵니다."));

	WeaponList = USKDataManager::Get()->GetWeaponList();

	int32 index = 0;
	for(const auto& Weapon : WeaponList)
	{
		UE_LOG(LogTemp, Log, TEXT("무기 로드 : %s"), *Weapon.WeaponName);
		
		CreateWeaponCard(Weapon.WeaponName, Weapon.WeaponThumbnail, Weapon.WeaponType, index);
		index++;
	}
}

void USKWeaponSelectionWidget::CreateWeaponCard(const FString& WeaponName, UTexture2D* Thumbnail, const FString& WeaponType, const int32 WeaponIndex)
{
	if(!WeaponGridPanel || !WBP_SKWeaponCard)
		return;

	if(USKWeaponCardWidget* WeaponCard = CreateWidget<USKWeaponCardWidget>(GetWorld(), WBP_SKWeaponCard))
	{
		WeaponCard->SetWeaponInfo(WeaponName, Thumbnail, WeaponType);
		WeaponCard->OnWeaponCardSelected.AddDynamic(this, &USKWeaponSelectionWidget::WeaponSelected);
		WeaponGridPanel->AddChildToUniformGrid(WeaponCard, WeaponIndex/2, WeaponIndex%2);
	}
}

void USKWeaponSelectionWidget::WeaponSelected(const FString& WeaponName)
{
	UE_LOG(LogTemp, Log, TEXT("선택된 무기 : %s"), *WeaponName);

	OnWeaponSelected.Broadcast(WeaponName);
}

