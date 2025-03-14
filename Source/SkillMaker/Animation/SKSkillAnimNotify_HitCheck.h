// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Skill/SKSkillData.h"
#include "SKSkillAnimNotify_HitCheck.generated.h"

class ASKBaseCharacter;

UCLASS()
class SKILLMAKER_API USKSkillAnimNotify_HitCheck : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
	/** 근접 타격 판정 */
	void PerformMeleeAttack(ASKBaseCharacter* Character, const FSKSkillData& SkillData);
};