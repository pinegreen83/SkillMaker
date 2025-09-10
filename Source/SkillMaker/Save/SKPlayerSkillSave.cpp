// Fill out your copyright notice in the Description page of Project Settings.


#include "SKPlayerSkillSave.h"

FSKSkillSet USKPlayerSkillSave::GetSkillSet(const FName& SkillSetName)
{
	return PlayerSkills[SkillSetName];
}

TMap<FName, FSKSkillSet> USKPlayerSkillSave::GetAllSkillSets()
{
	return PlayerSkills;
}

void USKPlayerSkillSave::SetSkillData(const FName& InSkillID, const FSKSkillData& InSkillData)
{
	CurrentSkillSet.Skills.Add(InSkillID, InSkillData);
}

void USKPlayerSkillSave::SetSkillSet(const FName& InSkillSetName)
{
	PlayerSkills.Add(InSkillSetName, CurrentSkillSet);
}
