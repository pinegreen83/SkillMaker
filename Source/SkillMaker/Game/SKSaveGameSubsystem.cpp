// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/SKSaveGameSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Logging/SKLogSkillMakerMacro.h"
#include "Save/SKPlayerSkillSave.h"
#include "Skill/SKSkillData.h"

void USKSaveGameSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	LoadData(DefaultSlotName, DefaultUserIndex);
}

bool USKSaveGameSubsystem::SaveSkillData(const FName InSkillID, const FSKSkillData& InSaveData)
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));

	if (InSkillID.IsNone())
	{
		SK_LOG(LogSkillMaker, Error, TEXT("SkillID가 None이라 저장할 수 없음."));
		return false;
	}

	USKPlayerSkillSave* SkillSave = GetOrCreatePlayerSkillSave();
	if (!SkillSave)
	{
		SK_LOG(LogSkillMaker, Error, TEXT("USKPlayerSkillSave 생성 실패."));
		return false;
	}

	if (!SkillSave->SetSkillData(InSkillID, InSaveData))
	{
		SK_LOG(LogSkillMaker, Error, TEXT("스킬 데이터 저장 실패: %s"), *InSkillID.ToString());
		return false;
	}

	return WritePlayerSkillSaveToSlot();
}

bool USKSaveGameSubsystem::SaveAllSkillSet()
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));

	return WritePlayerSkillSaveToSlot();
}

TArray<FSKSkillData> USKSaveGameSubsystem::GetSavedSkillList() const
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));

	if (!PlayerSkillSave)
	{
		SK_LOG(LogSkillMaker, Warning, TEXT("PlayerSkillSave가 nullptr임."));
		return TArray<FSKSkillData>();
	}

	return PlayerSkillSave->GetSavedSkillList();
}

bool USKSaveGameSubsystem::GetSkillDataByID(const FName& InSkillID, FSKSkillData& OutSkillData) const
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));

	if (!PlayerSkillSave)
	{
		SK_LOG(LogSkillMaker, Warning, TEXT("PlayerSkillSave가 nullptr임."));
		return false;
	}

	return PlayerSkillSave->GetSkillDataByID(InSkillID, OutSkillData);
}

FSKSkillSet USKSaveGameSubsystem::GetSkillSet(const FName& InSkillID)
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));

	if (!PlayerSkillSave)
	{
		SK_LOG(LogSkillMaker, Warning, TEXT("PlayerSkillSave가 nullptr임."));
		return FSKSkillSet();
	}

	return PlayerSkillSave->GetSkillSet(InSkillID);
}

void USKSaveGameSubsystem::LoadData(const FString& SlotName, int32 UserIndex)
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));

	DefaultSlotName = SlotName;
	DefaultUserIndex = UserIndex;

	if (UGameplayStatics::DoesSaveGameExist(SlotName, UserIndex))
	{
		PlayerSkillSave = Cast<USKPlayerSkillSave>(UGameplayStatics::LoadGameFromSlot(SlotName, UserIndex));

		if(PlayerSkillSave == nullptr)
		{
			SK_LOG(LogSkillMaker, Warning, TEXT("Failed to load SKPlayerSkillSave!"));
			return;
		}

		SK_LOG(LogSkillMaker, Log, TEXT("Loaded SaveGame Data"));
	}
	else
	{
		PlayerSkillSave = Cast<USKPlayerSkillSave>(UGameplayStatics::CreateSaveGameObject(USKPlayerSkillSave::StaticClass()));
		if (!PlayerSkillSave)
		{
			SK_LOG(LogSkillMaker, Error, TEXT("새 USKPlayerSkillSave 생성 실패."));
			return;
		}

		SK_LOG(LogSkillMaker, Log, TEXT("새 SaveGame Data 생성: %s"), *SlotName);
		WritePlayerSkillSaveToSlot();
	}
}

USKPlayerSkillSave* USKSaveGameSubsystem::GetOrCreatePlayerSkillSave()
{
	if (!PlayerSkillSave)
	{
		LoadData(DefaultSlotName, DefaultUserIndex);
	}

	if (!PlayerSkillSave)
	{
		PlayerSkillSave = Cast<USKPlayerSkillSave>(UGameplayStatics::CreateSaveGameObject(USKPlayerSkillSave::StaticClass()));
	}

	return PlayerSkillSave;
}

bool USKSaveGameSubsystem::WritePlayerSkillSaveToSlot()
{
	if (!PlayerSkillSave)
	{
		SK_LOG(LogSkillMaker, Error, TEXT("PlayerSkillSave가 nullptr이라 저장할 수 없음."));
		return false;
	}

	const bool bSaveSucceeded = UGameplayStatics::SaveGameToSlot(PlayerSkillSave, DefaultSlotName, DefaultUserIndex);
	if (!bSaveSucceeded)
	{
		SK_LOG(LogSkillMaker, Error, TEXT("SaveGame 저장 실패: %s / %d"), *DefaultSlotName, DefaultUserIndex);
		return false;
	}

	SK_LOG(LogSkillMaker, Log, TEXT("SaveGame 저장 완료: %s / %d"), *DefaultSlotName, DefaultUserIndex);
	return true;
}
