// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Save/SKPlayerSkillSave.h"
#include "SKSaveGameSubsystem.generated.h"

/**
 *
 */

class USKPlayerSkillSave;

UCLASS()
class SKILLMAKER_API USKSaveGameSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	bool SaveSkillData(const FName InSkillID, const FSKSkillData& InSaveData);

	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	bool SaveAllSkillSet();

	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	TArray<FSKSkillData> GetSavedSkillList() const;

	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	bool GetSkillDataByID(const FName& InSkillID, FSKSkillData& OutSkillData) const;

	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	FSKSkillSet GetSkillSet(const FName& InSkillID);

	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	void LoadData(const FString& SlotName, int32 UserIndex);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "SaveGame")
	TObjectPtr<USKPlayerSkillSave> PlayerSkillSave;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SaveGame")
	FString DefaultSlotName = TEXT("Test1");

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SaveGame")
	int32 DefaultUserIndex = 0;

private:
	USKPlayerSkillSave* GetOrCreatePlayerSkillSave();
	bool WritePlayerSkillSaveToSlot();
};
