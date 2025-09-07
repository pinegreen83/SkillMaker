// Fill out your copyright notice in the Description page of Project Settings.


#include "SKSkillTrainGameMode.h"
#include "Logging/SKLogSkillMakerMacro.h"
#include "Skill/SKSkillManager.h"

ASKSkillTrainGameMode::ASKSkillTrainGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassRef(TEXT("/Script/Engine.Blueprint'/Game/SkillMaker/Blueprint/BP_SKPlayerCharacter.BP_SKPlayerCharacter_C'"));
	if(DefaultPawnClassRef.Class != nullptr)
	{
		DefaultPawnClass = DefaultPawnClassRef.Class;
	}
	
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/Engine.Blueprint'/Game/SkillMaker/Blueprint/BP_SKPlayerController.BP_SKPlayerController_C'"));
	if(PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}
}

void ASKSkillTrainGameMode::BeginPlay()
{
	Super::BeginPlay();

	if(USKSkillManager::Get())
	{
		USKSkillManager::Get()->SkillDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/SkillMaker/Data/DT_SkillData.DT_SkillData"));

		if(USKSkillManager::Get()->SkillDataTable)
		{
			SK_LOG(LogSkillMaker, Log, TEXT("SkillDataTable 초기화 완료."));
		}
		else
		{
			SK_LOG(LogSkillMaker, Error, TEXT("SkillDataTable 초기화 실패!"));
		}
	}
}
