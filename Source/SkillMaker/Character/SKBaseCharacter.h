// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Skill/SKSkillData.h"
#include "SKBaseCharacter.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogSKCharacter, Log, All);

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
	/** 스킬 사용 함수(모든 캐릭터에서 공통) */
	UFUNCTION(BlueprintCallable, Category = "Skill")
	virtual void UseSkill(const FName& SkillID);

	/** 스킬 종료(애니메이션 종료 시점) */
	UFUNCTION()
	void EndSkillAction();

	/** 애님 노티파이 트리거 처리 */
	UFUNCTION()
	void HandleSkillNotify(FName EffectName, FName SoundName, FName StatusEffect, bool bTriggerHitDetection);
	
	/** 현재 사용중인 스킬 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill")
	TOptional<FSKSkillData> CurrentSkillData;

	/** 스킬 컴포넌트 */	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill")
	TObjectPtr<USKSkillComponent> SkillComponent;

	/** 전투 컴포넌트 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<USKCombatComponent> CombatComponent;

	FTimerHandle SkillEndTimer;

	TOptional<FSKSkillData> GetCurrentSkillData() const;
};
