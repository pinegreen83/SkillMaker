// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/SKSkillComponent.h"
#include "SKSkillManager.h"
#include "GameFramework/Character.h"
#include "Animation/AnimInstance.h"

// Sets default values for this component's properties
USKSkillComponent::USKSkillComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void USKSkillComponent::BeginPlay()
{
	Super::BeginPlay();
}

void USKSkillComponent::UseSkill(const FString& SkillName)
{
	if(!IsSkillAvailable(SkillName))
	{
		UE_LOG(LogTemp, Warning, TEXT("스킬 사용 불가 (쿨다운 중) : %s"), *SkillName);
		return;
	}

	TOptional<FSKSkillData> SkillData = USKSkillManager::Get()->GetSkillDataByName(SkillName);
	if(SkillData.IsSet())
	{
		HandleSkillExecution(SkillData.GetValue());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("스킬을 찾을 수 없음 : %s"), *SkillName);
	}
}

bool USKSkillComponent::IsSkillAvailable(const FString& SkillName) const
{
	const float* CooldownEnd = SkillCooldowns.Find(SkillName);
	return !CooldownEnd || *CooldownEnd <= GetWorld()->GetTimeSeconds();
}

void USKSkillComponent::HandleSkillExecution(const FSKSkillData& SkillData)
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if(!OwnerCharacter || !SkillData.SkillMontage)
	{
		UE_LOG(LogTemp, Error, TEXT("스킬 실행 불가 : 캐릭터 또는 애니메이션 몽타주 없음"));
		return;
	}

	UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
	if(AnimInstance)
	{
		AnimInstance->Montage_Play(SkillData.SkillMontage);
		UE_LOG(LogTemp, Log, TEXT("스킬 실행 : %s"), *SkillData.SkillName);
	}

	ApplyCooldown(SkillData.SkillName, SkillData.CooldownTime);
}

void USKSkillComponent::ApplyCooldown(const FString& SkillName, float CooldownTime)
{
	if(CooldownTime > 0.0f)
	{
		SkillCooldowns.Add(SkillName, GetWorld()->GetTimeSeconds() + CooldownTime);
		UE_LOG(LogTemp, Log, TEXT("쿨다운 적용 : %s, %f초"), *SkillName, CooldownTime);
	}
}