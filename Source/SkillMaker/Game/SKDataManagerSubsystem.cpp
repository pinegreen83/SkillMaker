// Fill out your copyright notice in the Description page of Project Settings.


#include "SKDataManagerSubsystem.h"
#include "Logging/SKLogSkillMakerMacro.h"

namespace
{
bool IsAnimationCompatibleWithWeapon(const FGameplayTag& WeaponTag, const FGameplayTagContainer& CompatibleWeaponTags)
{
	if (!WeaponTag.IsValid())
	{
		return false;
	}

	for (const FGameplayTag& CompatibleWeaponTag : CompatibleWeaponTags)
	{
		if (CompatibleWeaponTag.IsValid() && WeaponTag.MatchesTag(CompatibleWeaponTag))
		{
			return true;
		}
	}

	return false;
}
}

void USKDataManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	LoadDefaultDataTables();
}

void USKDataManagerSubsystem::LoadDefaultDataTables()
{
	if (!WeaponDataTable)
	{
		WeaponDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/SkillMaker/Data/DT_WeaponData.DT_WeaponData"));
	}

	if (!AnimationDataTable)
	{
		AnimationDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/SkillMaker/Data/DT_AnimationData.DT_AnimationData"));
	}

	if (!ProjectileDataTable)
	{
		ProjectileDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/SkillMaker/Data/DT_ProjectileData.DT_ProjectileData"));
	}

	SK_LOG(LogSkillMaker, Log, TEXT("DataTable load result - Weapon: %s, Animation: %s, Projectile: %s"),
		WeaponDataTable ? TEXT("Valid") : TEXT("Invalid"),
		AnimationDataTable ? TEXT("Valid") : TEXT("Invalid"),
		ProjectileDataTable ? TEXT("Valid") : TEXT("Invalid"));
}

TArray<FSKWeaponRow> USKDataManagerSubsystem::GetWeaponList()
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));
	
	TArray<FSKWeaponRow> WeaponList;

	if(!WeaponDataTable)
	{
		SK_LOG(LogSkillMaker, Error, TEXT("WeaponDataTable이 nullptr임."));
		return WeaponList;
	}

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

TArray<FSKAnimationRow> USKDataManagerSubsystem::GetAnimationsForWeapon(FGameplayTag WeaponTag)
{
	SK_LOG(LogSkillMaker, Log, TEXT("무기 태그에 맞는 애니메이션 조회: %s"), *WeaponTag.ToString());
	
	TArray<FSKAnimationRow> AnimationList;
	if (!WeaponTag.IsValid())
	{
		SK_LOG(LogSkillMaker, Warning, TEXT("유효하지 않은 무기 태그가 전달됨."));
		return AnimationList;
	}

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
			if(IsAnimationCompatibleWithWeapon(WeaponTag, AnimationData->CompatibleWeaponTags))
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
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));

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
