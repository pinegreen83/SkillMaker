// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SKSaveGameSubsystem.generated.h"

/**
 * 
 */

struct FSKSkillData;
struct FSKSkillSet;
class USKSaveSkillSave;
class USKPlayerSkillSave;

UCLASS()
class SKILLMAKER_API USKSaveGameSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	void SaveSkillData(const FName InSkillID, const FSKSkillData& InSaveData);
	
	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	void SaveAllSkillSet();

	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	FSKSkillSet GetSkillSet(const FName& InSkillID);
	
	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	void LoadData(const FString& SlotName, int32 UserIndex);
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "SaveGame")
	USKPlayerSkillSave* PlayerSkillSave;
};
