// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/SKDataManagerSubsystem.h"
#include "Logging/SKLogSkillMakerMacro.h"

TArray<FSKWeaponRow> USKDataManagerSubsystem::GetWeaponList()
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));
	
	TArray<FSKWeaponRow> WeaponList;

	if(!WeaponDataTable)
		return WeaponList;

	TArray<FName> RowNames = WeaponDataTable->GetRowNames();
	for(const FName RowName : RowNames)
	{
		if(const FSKWeaponData* WeaponData = WeaponDataTable->FindRow<FSKWeaponData>(RowName, TEXT("")))
		{
			FSKWeaponRow NowWeaponRow(RowName, *WeaponData);
			WeaponList.Add(NowWeaponRow);
		}
	}

	return WeaponList;
}

TArray<FSKAnimationRow> USKDataManagerSubsystem::GetAnimationsForWeapon(const FString& WeaponType)
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));
	
	TArray<FSKAnimationRow> AnimationList;

	if(!AnimationDataTable)
	{
		SK_LOG(LogSkillMaker, Error, TEXT("AnimationDataTable이 nullptr임."));
		return AnimationList;	
	}

	TArray<FName> RowNames = AnimationDataTable->GetRowNames();
	for(const FName RowName : RowNames)
	{
		if(const FSKAnimationData* AnimationData = AnimationDataTable->FindRow<FSKAnimationData>(RowName, TEXT("")))
		{
			if(AnimationData->WeaponType == WeaponType)
			{
				FSKAnimationRow NowAnimationRow(RowName, *AnimationData);
				AnimationList.Add(NowAnimationRow);
			}
		}
	}

	return AnimationList;
}

TArray<FSKProjectileRow> USKDataManagerSubsystem::GetProjectileList()
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"))

TArray<FSKProjectileRow> ProjectileList;

	if(!ProjectileDataTable)
	{
		SK_LOG(LogSkillMaker, Log, TEXT("ProjectileDataTable이 nullptr임."));
		return ProjectileList;
	}

	TArray<FName> RowNames = ProjectileDataTable->GetRowNames();
	for(const FName RowName : RowNames)
	{
		if(const FSKProjectileData* ProjectileData = ProjectileDataTable->FindRow<FSKProjectileData>(RowName, TEXT("")))
		{
			FSKProjectileRow NowProjectileRow(RowName, *ProjectileData);
			ProjectileList.Add(NowProjectileRow);
		}
	}
	
	return ProjectileList;
}
