// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Skill/SKSkillData.h"
#include "SKBaseCharacter.generated.h"

class USKSkillComponent;
class USKCombatComponent;

UCLASS()
class SKILLMAKER_API ASKBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASKBaseCharacter();

protected:
	virtual void BeginPlay() override;

public:
	/** 스킬 사용 */
	TOptional<FSKSkillData> GetCurrentSkillData() { return CurrentSkillData; }
	void SetCurrentSkillData(const TOptional<FSKSkillData>& SkillData) { CurrentSkillData = SkillData; };
	
	/** 스킬 컴포넌트에 스킬 설정 */
	void SetSkillDataInMap(const FSKSkillData& SkillData);
	
	void SetSkillDataInComponent(const TMap<FName, FSKSkillData>& SkillDataMap);
	
	/** 스킬 사용 함수(모든 캐릭터에서 공통) */
	UFUNCTION(BlueprintCallable, Category = "Skill")
	virtual void UseSkill(const FName& SkillID);

	/** 전투 컴포넌트 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<USKCombatComponent> CombatComponent;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill")
	TOptional<FSKSkillData> CurrentSkillData;
	
	/** 스킬 컴포넌트 */	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill")
	TObjectPtr<USKSkillComponent> SkillComponent;
};
