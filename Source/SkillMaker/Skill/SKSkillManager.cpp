// Fill out your copyright notice in the Description page of Project Settings.


#include "SKSkillManager.h"
#include "SKSkillData.h"
#include "Logging/SKLogSkillMakerMacro.h"

USKSkillManager* USKSkillManager::Instance = nullptr;

USKSkillManager* USKSkillManager::Get()
{
	if(!Instance)
	{
		Instance = NewObject<USKSkillManager>();
	}

	return Instance;
}

TArray<FSKSkillData> USKSkillManager::GetSkillList()
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));
	
	TArray<FSKSkillData> SkillList;

	if(!SkillDataTable)
	{
		SK_LOG(LogSkillMaker, Error, TEXT("SkillDataTable이 nullptr임"));
		return SkillList;
	}

	TArray<FName> RowNames = SkillDataTable->GetRowNames();
	for(FName RowName : RowNames)
	{
		if(FSKSkillData* Row = SkillDataTable->FindRow<FSKSkillData>(RowName, TEXT("")))
		{
			SkillList.Add(*Row);
		}		
	}

	return SkillList;
}

TOptional<FSKSkillData> USKSkillManager::GetSkillDataByID(const FName& SkillID)
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));
	if(!SkillDataTable)
		return TOptional<FSKSkillData>();

	TArray<FName> RowNames = SkillDataTable->GetRowNames();
	for(FName RowName : RowNames)
	{
		if(FSKSkillData* Row = SkillDataTable->FindRow<FSKSkillData>(RowName, TEXT("")))
		{
			SK_LOG(LogSkillMaker, Log, TEXT("스킬 이름 : %s 발견"), *Row->SkillName);
			if(Row->SkillName == SkillID)
			{
				SK_LOG(LogSkillMaker, Log, TEXT("스킬 이름 : %s 세팅 완료."), *Row->SkillName);
				return *Row;
			}
		}
	}

	return TOptional<FSKSkillData>();
}

void USKSkillManager::SaveSkillData(const FSKSkillData& SkillData)
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));
	if(!SkillDataTable)
		return;

	FSKSkillData* ExistingSkill = SkillDataTable->FindRow<FSKSkillData>(SkillData.SkillID, TEXT(""));
	if (ExistingSkill)
	{
		*ExistingSkill = SkillData;
		SK_LOG(LogSkillMaker, Log, TEXT("스킬 데이터 업데이트 완료: %s"), *SkillData.SkillName);
	}
	else
	{
		// 새로운 스킬 추가
		SkillDataTable->AddRow(SkillData.SkillID, SkillData);
		if (SkillDataTable->FindRow<FSKSkillData>(SkillData.SkillID, TEXT("")))
		{
			SK_LOG(LogSkillMaker, Log, TEXT("새로운 스킬 저장 완료: %s"), *SkillData.SkillID.ToString());	
		}
	}
}
