// Fill out your copyright notice in the Description page of Project Settings.


#include "SKSkillManager.h"
#include "SKSkillData.h"

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
	TArray<FSKSkillData> SkillList;

	if(!SkillDataTable)
	{
		UE_LOG(LogTemp, Error, TEXT("SkillDataTable이 nullptr임"));
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
	if(!SkillDataTable)
	{
		UE_LOG(LogTemp, Log, TEXT("SkillDataTable이 nullptr임."));
		return TOptional<FSKSkillData>();
	}

	TArray<FName> RowNames = SkillDataTable->GetRowNames();
	for(FName RowName : RowNames)
	{
		if(FSKSkillData* Row = SkillDataTable->FindRow<FSKSkillData>(RowName, TEXT("")))
		{
			UE_LOG(LogTemp, Log, TEXT("스킬 이름 : %s"), *Row->SkillName);
			if(Row->SkillID == SkillID)
			{
				return *Row;
			}
		}
	}

	return TOptional<FSKSkillData>();
}

void USKSkillManager::SaveSkillData(const FSKSkillData& SkillData)
{
	if(!SkillDataTable)
	{
		UE_LOG(LogTemp, Log, TEXT("SkillDataTable이 nullptr임."));
		return;
	}

	FSKSkillData* ExistingSkill = SkillDataTable->FindRow<FSKSkillData>(SkillData.SkillID, TEXT(""));
	if (ExistingSkill)
	{
		*ExistingSkill = SkillData;
		UE_LOG(LogTemp, Log, TEXT("스킬 데이터 업데이트 완료: %s"), *SkillData.SkillName);
	}
	else
	{
		// 새로운 스킬 추가
		SkillDataTable->AddRow(SkillData.SkillID, SkillData);
		UE_LOG(LogTemp, Log, TEXT("새로운 스킬 저장 완료: %s"), *SkillData.SkillName);
	}
}
