// Fill out your copyright notice in the Description page of Project Settings.


#include "SKBaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"
#include "Combat/SKCombatComponent.h"
#include "Logging/SKLogSkillMakerMacro.h"
#include "Skill/SKSkillComponent.h"

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

void ASKBaseCharacter::SetSkillDataInMap(const FSKSkillData& SkillData)
{
	SkillComponent->SetSkillDataInMap(SkillData);
}

void ASKBaseCharacter::SetSkillDataInComponent(const TMap<FName, FSKSkillData>& SkillDataMap)
{
	SkillComponent->SetSkillMapInComponent(SkillDataMap);
}

void ASKBaseCharacter::UseSkill(const FName& SkillID)
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));
	
	if (!SkillComponent) return;

	SK_LOG(LogSkillMaker, Log, TEXT("스킬 실행: %s"), *SkillID.ToString());
	SkillComponent->ClientRequestUseSkill(SkillID);
}
