// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SKSkillData.h"
#include "GameFramework/SaveGame.h"
#include "SKPlayerSkillSave.generated.h"

/**
 * 
 */
UCLASS()
class SKILLMAKER_API USKPlayerSkillSave : public USaveGame
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "PlayerSkill")
	FSKSkillData GetSkillData(const FName& InSkillID) { return PlayerSkills[InSkillID]; }

	UFUNCTION(BlueprintCallable, Category = "PlayerSkill")
	TMap<FName, FSKSkillData> GetAllSkillData() { return PlayerSkills; }
	
	UFUNCTION(BlueprintCallable, Category = "PlayerSkill")
	void SetSkillData(const FName& InSkillID, const FSKSkillData& InSkillData) { PlayerSkills.Add({InSkillID, InSkillData}); }

protected:
	UPROPERTY(VisibleAnywhere, Category = "PlayerSkill")
	TMap<FName, FSKSkillData> PlayerSkills;
};
