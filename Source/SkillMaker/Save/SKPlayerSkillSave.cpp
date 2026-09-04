// Fill out your copyright notice in the Description page of Project Settings.


#include "SKPlayerSkillSave.h"

FSKSkillSet USKPlayerSkillSave::GetSkillSet(const FName& SkillSetName)
{
	if (const FSKSkillSet* SkillSet = PlayerSkills.Find(SkillSetName))
	{
		return *SkillSet;
	}

	return FSKSkillSet();
}

TMap<FName, FSKSkillSet> USKPlayerSkillSave::GetAllSkillSets()
{
	return PlayerSkills;
}

TArray<FSKSkillData> USKPlayerSkillSave::GetSavedSkillList() const
{
	TArray<FSKSkillData> SkillList;
	CurrentSkillSet.Skills.GenerateValueArray(SkillList);
	return SkillList;
}

bool USKPlayerSkillSave::GetSkillDataByID(const FName& InSkillID, FSKSkillData& OutSkillData) const
{
	if (const FSKSkillData* SkillData = CurrentSkillSet.Skills.Find(InSkillID))
	{
		OutSkillData = *SkillData;
		return true;
	}

	return false;
}

bool USKPlayerSkillSave::HasSkillData(const FName& InSkillID) const
{
	return CurrentSkillSet.Skills.Contains(InSkillID);
}

bool USKPlayerSkillSave::SetSkillData(const FName& InSkillID, const FSKSkillData& InSkillData)
{
	if (InSkillID.IsNone())
	{
		return false;
	}

	FSKSkillData SaveData = InSkillData;
	SaveData.SkillID = InSkillID;
	CurrentSkillSet.Skills.Add(InSkillID, SaveData);
	return true;
}

void USKPlayerSkillSave::SetSkillSet(const FName& InSkillSetName)
{
	PlayerSkills.Add(InSkillSetName, CurrentSkillSet);
}
