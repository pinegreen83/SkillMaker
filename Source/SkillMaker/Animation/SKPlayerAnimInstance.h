// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SKPlayerAnimInstance.generated.h"

class UBlendSpace;
class UAnimMontage;
class ASKBaseCharacter;

UCLASS()
class SKILLMAKER_API USKPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	USKPlayerAnimInstance();
	
	/** 스킬 사용 중인지 여부 */
	UPROPERTY(BlueprintReadOnly, Category = "Skill")
	bool bIsCastingSkill;

	/** 스킬 사용 중 이동 가능한지 여부 */
	UPROPERTY(BlueprintReadOnly, Category = "Skill")
	bool bCanMoveWhileCasting;
	
	/** 이동 중 스킬 애니메이션 적용 여부 */
	UPROPERTY(BlueprintReadOnly, Category = "Skill")
	bool bUseMoveBlendSpace;

	/** 기본 이동 블렌드 스페이스 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	TObjectPtr<UBlendSpace> DefaultMoveBlendSpace;
	
	/** 이동 중 스킬 애니메이션 블렌드 스페이스 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	TObjectPtr<UBlendSpace> MoveSkillBlendSpace;
	
	/** 스킬 애니메이션 상태 설정 */
	void SetSkillAnimationState(bool bCasting, bool bCanMove, bool bUseBlendSpace, UBlendSpace* SkillBlendSpace = nullptr);

	/** 스킬 애니메이션 중단 */
	void StopSkillAnimation();
	
protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	/** 현재 사용 중인 블렌드 스페이스 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBlendSpace> CurrentBlendSpace;

	/** 현재 애니메이션을 실행 중인 캐릭터 */
	UPROPERTY()
	TObjectPtr<ASKBaseCharacter> OwnerCharacter;
};
