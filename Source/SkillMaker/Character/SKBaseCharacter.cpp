// Fill out your copyright notice in the Description page of Project Settings.


#include "SKBaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/SKPlayerAnimInstance.h"
#include "Combat/SKCombatComponent.h"
#include "Logging/SKLogSkillMakerMacro.h"
#include "Skill/SKSkillComponent.h"
#include "Skill/SKSkillManager.h"

// Sets default values
ASKBaseCharacter::ASKBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	/** 캡슐 설정 */
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	/** 회전 및 이동 설정 */
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	
	GetCharacterMovement()->JumpZVelocity = 700.0f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	/** 메시 설정 */
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

	/** 기본 애셋 로드 */
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/UEAsset/Characters/Mannequins/Meshes/SKM_Manny.SKM_Manny'"));
	if(CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/UEAsset/Characters/Mannequins/Animations/ABP_Manny.ABP_Manny_C"));
	if(AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}
	
	/** 스킬 컴포넌트 생성 */
	SkillComponent = CreateDefaultSubobject<USKSkillComponent>(TEXT("SkillComponent"));
	
	/** 전투 컴포넌트 생성 */
	CombatComponent = CreateDefaultSubobject<USKCombatComponent>(TEXT("CombatComponent"));
}

void ASKBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ASKBaseCharacter::UseSkill(const FName& SkillID)
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));
	
	if (!SkillComponent) return;

	if (CurrentSkillData.IsSet())
	{
		SK_LOG(LogSkillMaker, Log, TEXT("스킬 실행: %s"), *CurrentSkillData->SkillName);
		
		SkillComponent->ClientRequestUseSkill(SkillID);

		if (CurrentSkillData->SkillMontage)
		{
			PlayAnimMontage(CurrentSkillData->SkillMontage);
		}

		if (CurrentSkillData->SkillDuration > 0.0f)
		{
			GetWorld()->GetTimerManager().SetTimer(
				SkillEndTimer,
				[this]() { EndSkillAction(); },
				CurrentSkillData->SkillDuration,
				false
			);
		}
	}
}

void ASKBaseCharacter::EndSkillAction()
{
	SK_LOG(LogSkillMaker, Log, TEXT("End"));
	
	// 애니메이션 상태 해제 (이동 가능하도록 설정)
	if (USKPlayerAnimInstance* AnimInstance = Cast<USKPlayerAnimInstance>(GetMesh()->GetAnimInstance()))
	{
		AnimInstance->SetSkillAnimationState(false, false, false);
	}

	CurrentSkillData.Reset();
	
	// 타이머 해제
	GetWorld()->GetTimerManager().ClearTimer(SkillEndTimer);
	
	SK_LOG(LogSkillMaker, Log, TEXT("스킬 종료 - CurrentSkillData 초기화 완료."));
}

void ASKBaseCharacter::HandleSkillNotify(FName EffectName, FName SoundName, FName StatusEffect, bool bTriggerHitDetection)
{
	SK_LOG(LogSkillMaker, Log, TEXT("애님 노티파이 처리: 이펙트 %s, 사운드 %s, 상태이상 %s, 타격판정 %s"),
		*EffectName.ToString(), *SoundName.ToString(), *StatusEffect.ToString(), bTriggerHitDetection ? TEXT("True") : TEXT("False"));

	// 이펙트 실행
	if (!EffectName.IsNone())
	{
		// TODO: 이펙트 스폰 로직 추가
	}

	// 사운드 실행
	if (!SoundName.IsNone())
	{
		// TODO: 사운드 플레이 로직 추가
	}

	// 상태 이상 처리 (ex. 슬로우, 스턴 등)
	if (!StatusEffect.IsNone())
	{
		// TODO: 상태이상 적용 로직 추가
	}

	// 타격 판정 실행
	if (bTriggerHitDetection)
	{
		// TODO: 타격 판정 실행
	}
}

TOptional<FSKSkillData> ASKBaseCharacter::GetCurrentSkillData() const
{
	return CurrentSkillData;
}

void ASKBaseCharacter::SetSkillData(FSKSkillData SkillData)
{
	CurrentSkillData = SkillData;
}
