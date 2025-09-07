// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/SKDataManager.h"
#include "SKWeaponData.h"
#include "SKAnimationData.h"
#include "Engine/DataTable.h"
#include "Logging/SKLogSkillMakerMacro.h"

USKDataManager* USKDataManager::Instance = nullptr;

USKDataManager* USKDataManager::Get()
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));
	
	if(!Instance)
	{
		Instance = NewObject<USKDataManager>();
	}

	return Instance;
}

TArray<FSKWeaponData> USKDataManager::GetWeaponList()
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));
	
	TArray<FSKWeaponData> WeaponList;

	if(!WeaponDataTable)
		return WeaponList;

	TArray<FName> RowNames = WeaponDataTable->GetRowNames();
	for(FName RowName : RowNames)
	{
		if(FSKWeaponData* Row = WeaponDataTable->FindRow<FSKWeaponData>(RowName, TEXT("")))
		{
			WeaponList.Add(*Row);
		}
	}

	return WeaponList;
}

TArray<FSKAnimationData> USKDataManager::GetAnimationsForWeapon(const FString& WeaponType)
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));
	
	TArray<FSKAnimationData> AnimationList;

	if(!AnimationDataTable)
	{
		SK_LOG(LogSkillMaker, Error, TEXT("AnimationDataTable이 nullptr임."));
		return AnimationList;	
	}

	TArray<FName> RowNames = AnimationDataTable->GetRowNames();
	for(FName RowName : RowNames)
	{
		if(FSKAnimationData* Row = AnimationDataTable->FindRow<FSKAnimationData>(RowName, TEXT("")))
		{
			if(Row->WeaponType == WeaponType)
			{
				AnimationList.Add(*Row);
			}
		}
	}

	return AnimationList;
}
