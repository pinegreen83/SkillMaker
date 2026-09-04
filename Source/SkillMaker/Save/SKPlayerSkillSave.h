// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Skill/SKSkillData.h"
#include "SKPlayerSkillSave.generated.h"

/**
 *
 */

USTRUCT(BlueprintType)
struct FSKSkillSet
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TMap<FName, FSKSkillData> Skills;
};

UCLASS()
class SKILLMAKER_API USKPlayerSkillSave : public USaveGame
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "PlayerSkill")
	FSKSkillSet GetSkillSet(const FName& SkillSetName);

	UFUNCTION(BlueprintCallable, Category = "PlayerSkill")
	TMap<FName, FSKSkillSet> GetAllSkillSets();

	UFUNCTION(BlueprintCallable, Category = "PlayerSkill")
	TArray<FSKSkillData> GetSavedSkillList() const;

	UFUNCTION(BlueprintCallable, Category = "PlayerSkill")
	bool GetSkillDataByID(const FName& InSkillID, FSKSkillData& OutSkillData) const;

	UFUNCTION(BlueprintCallable, Category = "PlayerSkill")
	bool HasSkillData(const FName& InSkillID) const;

	UFUNCTION(BlueprintCallable, Category = "PlayerSkill")
	bool SetSkillData(const FName& InSkillID, const FSKSkillData& InSkillData);

	UFUNCTION(BlueprintCallable, Category = "PlayerSkill")
	void SetSkillSet(const FName& InSkillSetName);

protected:
	UPROPERTY(EditAnywhere, Category = "SaveGame", SaveGame)
	FSKSkillSet CurrentSkillSet;

	UPROPERTY(VisibleAnywhere, Category = "PlayerSkill", SaveGame)
	TMap<FName, FSKSkillSet> PlayerSkills;
};
