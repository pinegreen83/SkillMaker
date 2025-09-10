// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/SKSaveGameSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Logging/SKLogSkillMakerMacro.h"
#include "Save/SKPlayerSkillSave.h"
#include "Skill/SKSkillData.h"

void USKSaveGameSubsystem::SaveSkillData(const FName InSkillID, const FSKSkillData& InSaveData)
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));
	
	if (!PlayerSkillSave) return;

	PlayerSkillSave->SetSkillData(InSkillID, InSaveData);
}

void USKSaveGameSubsystem::SaveAllSkillSet()
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));
	
	if (!PlayerSkillSave) return;

	UGameplayStatics::SaveGameToSlot(PlayerSkillSave, "Test1", 0);
}

FSKSkillSet USKSaveGameSubsystem::GetSkillSet(const FName& InSkillID)
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));

	return PlayerSkillSave->GetSkillSet(InSkillID);
}

void USKSaveGameSubsystem::LoadData(const FString& SlotName, int32 UserIndex)
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));
	
	if (UGameplayStatics::DoesSaveGameExist(SlotName, UserIndex))
	{
		PlayerSkillSave = Cast<USKPlayerSkillSave>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));

		if(PlayerSkillSave == nullptr)
		{
			SK_LOG(LogSkillMaker, Warning, TEXT("Failed to load SKPlayerSkillSave!"));
			return;
		}

		SK_LOG(LogSkillMaker, Log, TEXT("Loaded SaveGame Data"));
	}
}
