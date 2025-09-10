// Fill out your copyright notice in the Description page of Project Settings.


#include "SKSkillMakerGameMode.h"
#include "UI/UI-SkillMaker/SKSkillMakerHUD.h"
#include "Data/SKDataManager.h"
#include "Logging/SKLogSkillMakerMacro.h"
#include "Skill/SKSkillManager.h"

ASKSkillMakerGameMode::ASKSkillMakerGameMode()
{
	HUDClass = ASKSkillMakerHUD::StaticClass();
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/Engine.Blueprint'/Game/SkillMaker/Blueprint/Controller/BP_SKSkillMakerController.BP_SKSkillMakerController_C'"));
	if(PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}
}

void ASKSkillMakerGameMode::BeginPlay()
{
	Super::BeginPlay();

	// USKDataManager::Get();
	//
	// if (USKDataManager::Get())
	// {
	// 	USKDataManager::Get()->WeaponDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/SkillMaker/Data/DT_WeaponData.DT_WeaponData"));
	// 	USKDataManager::Get()->AnimationDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/SkillMaker/Data/DT_AnimationData.DT_AnimationData"));
	// 	USKDataManager::Get()->ProjectileDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/SkillMaker/Data/DT_ProjectileData.DT_ProjectileData"));
	//
	// 	if (USKDataManager::Get()->WeaponDataTable)
	// 	{
	// 		SK_LOG(LogSkillMaker, Warning, TEXT("WeaponDataTable 초기화 완료."));
	// 	}
	// 	else
	// 	{
	// 		SK_LOG(LogSkillMaker, Error, TEXT("WeaponDataTable 초기화 실패!"));
	// 	}
	//
	// 	if (USKDataManager::Get()->AnimationDataTable)
	// 	{
	// 		SK_LOG(LogSkillMaker, Warning, TEXT("AnimationDataTable 초기화 완료."));
	// 	}
	// 	else
	// 	{
	// 		SK_LOG(LogSkillMaker, Error, TEXT("AnimationDataTable 초기화 실패!"));
	// 	}
	// 	if (USKDataManager::Get()->ProjectileDataTable)
	// 	{
	// 		SK_LOG(LogSkillMaker, Warning, TEXT("ProjectileDataTable 초기화 완료."));
	// 	}
	// 	else
	// 	{
	// 		SK_LOG(LogSkillMaker, Error, TEXT("ProjectileDataTable 초기화 실패!"));
	// 	}
	// }
	//
	// USKSkillManager::Get();
	//
	// if(USKSkillManager::Get())
	// {
	// 	USKSkillManager::Get()->SkillDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/SkillMaker/Data/DT_SkillData.DT_SkillData"));
	//
	// 	if(USKSkillManager::Get()->SkillDataTable)
	// 	{
	// 		SK_LOG(LogSkillMaker, Warning, TEXT("SkillDataTable 초기화 완료."));
	// 	}
	// 	else
	// 	{
	// 		SK_LOG(LogSkillMaker, Error, TEXT("SkillDataTable 초기화 실패!"));
	// 	}
	// }
}
