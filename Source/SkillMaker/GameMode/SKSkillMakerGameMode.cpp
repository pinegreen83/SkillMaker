// Fill out your copyright notice in the Description page of Project Settings.


#include "SKSkillMakerGameMode.h"
#include "UI/SKSkillMakerHUD.h"
#include "Data/SKDataManager.h"
#include "Skill/SKSkillManager.h"

ASKSkillMakerGameMode::ASKSkillMakerGameMode()
{
	HUDClass = ASKSkillMakerHUD::StaticClass();
}

void ASKSkillMakerGameMode::BeginPlay()
{
	Super::BeginPlay();

	USKDataManager::Get();

	if (USKDataManager::Get())
	{
		USKDataManager::Get()->WeaponDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/SkillMaker/Data/DT_WeaponData.DT_WeaponData"));
		USKDataManager::Get()->AnimationDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/SkillMaker/Data/DT_AnimationData.DT_AnimationData"));

		if (USKDataManager::Get()->WeaponDataTable)
		{
			UE_LOG(LogTemp, Warning, TEXT("WeaponDataTable 초기화 완료."));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("WeaponDataTable 초기화 실패!"));
		}

		if (USKDataManager::Get()->AnimationDataTable)
		{
			UE_LOG(LogTemp, Warning, TEXT("AnimationDataTable 초기화 완료."));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("AnimationDataTable 초기화 실패!"));
		}
	}

	USKSkillManager::Get();

	if(USKSkillManager::Get())
	{
		USKSkillManager::Get()->SkillDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/SkillMaker/Data/DT_SkillData.DT_SkillData"));

		if(USKSkillManager::Get()->SkillDataTable)
		{
			UE_LOG(LogTemp, Warning, TEXT("SkillDataTable 초기화 완료."));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("SkillDataTable 초기화 실패!"));
		}
	}
}
