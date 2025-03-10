// Fill out your copyright notice in the Description page of Project Settings.


#include "SKBaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Skill/SKSkillData.h"

DEFINE_LOG_CATEGORY(LogSKCharacter);

// Sets default values
ASKBaseCharacter::ASKBaseCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	
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

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

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
}

void ASKBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ASKBaseCharacter::UseSkillAsset(USKSkillData* SkillDataAsset)
{
	if(!SkillDataAsset) return;

	const FSkillData& Skill = SkillDataAsset->SkillData;
	UE_LOG(LogTemp, Log, TEXT("Using Skill: %s (Damage = %.2f)"), *Skill.SkillName.ToString(), Skill.Damage);

	if(Skill.SkillMontage && GetMesh() && GetMesh()->GetAnimInstance())
	{
		GetMesh()->GetAnimInstance()->Montage_Play(Skill.SkillMontage);
	}
}

