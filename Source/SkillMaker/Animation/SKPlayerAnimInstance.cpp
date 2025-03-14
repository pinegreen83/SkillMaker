// Fill out your copyright notice in the Description page of Project Settings.


#include "SKPlayerAnimInstance.h"

#include "Character/SKBaseCharacter.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

USKPlayerAnimInstance::USKPlayerAnimInstance()
{
	bIsCastingSkill = false;
	bCanMoveWhileCasting = false;
	bUseMoveBlendSpace = false;
	CurrentBlendSpace = nullptr;
	OwnerCharacter = nullptr;
}

void USKPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	OwnerCharacter = Cast<ASKBaseCharacter>(TryGetPawnOwner());
}

void USKPlayerAnimInstance::SetSkillAnimationState(bool bCasting, bool bCanMove, bool bUseBlendSpace, UBlendSpace* SkillBlendSpace)
{
	bIsCastingSkill = bCasting;
	bCanMoveWhileCasting = bCanMove;
	bUseMoveBlendSpace = bUseBlendSpace;

	if(bIsCastingSkill && bCanMoveWhileCasting && bUseMoveBlendSpace && SkillBlendSpace)
	{
		CurrentBlendSpace = SkillBlendSpace;
	}
	else
	{
		CurrentBlendSpace = DefaultMoveBlendSpace;
	}
}

void USKPlayerAnimInstance::StopSkillAnimation()
{
	Montage_Stop(0.2f);
	bIsCastingSkill = false;
	bCanMoveWhileCasting = false;
	bUseMoveBlendSpace = false;

	CurrentBlendSpace = DefaultMoveBlendSpace;
}


void USKPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	if (!OwnerCharacter)
		return;

	UCharacterMovementComponent* MovementComponent = OwnerCharacter->GetCharacterMovement();
	if (!MovementComponent)
		return;

	if (bIsCastingSkill && bCanMoveWhileCasting && bUseMoveBlendSpace && MoveSkillBlendSpace)
	{
		CurrentBlendSpace = MoveSkillBlendSpace;
	}
	else
	{
		CurrentBlendSpace = DefaultMoveBlendSpace;
	}
}
