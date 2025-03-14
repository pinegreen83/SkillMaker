// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/SKSkillComponent.h"
#include "SKSkillManager.h"
#include "GameFramework/Character.h"
#include "Animation/AnimInstance.h"

// Sets default values for this component's properties
USKSkillComponent::USKSkillComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void USKSkillComponent::ClientRequestUseSkill(const FName& SkillID)
{
	if(GetOwner()->HasAuthority())
	{
		TOptional<FSKSkillData> SkillData = USKSkillManager::Get()->GetSkillDataByID(SkillID);
		if(SkillData.IsSet())
		{
			ExecuteSkill(SkillData.GetValue());
		}
	}
	else
	{
		ServerUseSkill(SkillID);
	}
}

bool USKSkillComponent::ServerUseSkill_Validate(const FName& SkillID)
{
	return SkillID.IsValid();
}

void USKSkillComponent::ServerUseSkill_Implementation(const FName& SkillID)
{
	if(!IsSkillAvailable(SkillID))
	{
		UE_LOG(LogTemp, Warning, TEXT("스킬 사용 불가(쿨다운 중) : %s"), *SkillID.ToString());
		return;
	}

	TOptional<FSKSkillData> SkillData = USKSkillManager::Get()->GetSkillDataByID(SkillID);
	if(SkillData.IsSet())
	{
		ExecuteSkill(SkillData.GetValue());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("스킬 데이터를 찾을 수 없음 : %s"), *SkillID.ToString());
	}
}

void USKSkillComponent::MulticastExecuteSkill_Implementation(const FSKSkillData& SkillData)
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if(!OwnerCharacter || !SkillData.SkillMontage)
	{
		UE_LOG(LogTemp, Error, TEXT("스킬 실행 불가 : 캐릭터 또는 애니메이션 몽타주 없음."));
		return;
	}

	UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
	if(AnimInstance)
	{
		AnimInstance->Montage_Play(SkillData.SkillMontage);
		UE_LOG(LogTemp, Log, TEXT("애니메이션 실행 : %s"), *SkillData.SkillName);
	}
}

void USKSkillComponent::ExecuteSkill(const FSKSkillData& SkillData)
{
	MulticastExecuteSkill(SkillData);

	ApplyCooldown(SkillData.SkillID, SkillData.CooldownTime);
}

void USKSkillComponent::ApplyCooldown(const FName& SkillID, float CooldownTime)
{
	if(CooldownTime > 0.0f)
	{
		float CooldownEndTime = GetWorld()->GetTimeSeconds() + CooldownTime;
		SkillCooldowns.Add(SkillID, CooldownEndTime);

		FTimerHandle CooldownTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(
			CooldownTimerHandle,
			[this, SkillID]() { ClearCooldown(SkillID); },
			CooldownTime,
			false
		);

		CooldownTimers.Add(SkillID, CooldownTimerHandle);
		UE_LOG(LogTemp, Log, TEXT("쿨다운 적용 : %s, %f초 후 해제"), *SkillID.ToString(), CooldownTime);
	}
}

void USKSkillComponent::ClearCooldown(FName SkillID)
{
	SkillCooldowns.Remove(SkillID);
	CooldownTimers.Remove(SkillID);
	UE_LOG(LogTemp, Log, TEXT("쿨다운 해제 됨 : %s"), *SkillID.ToString());
}

bool USKSkillComponent::IsSkillAvailable(const FName& SkillID) const
{
	const float* CooldownEnd = SkillCooldowns.Find(SkillID);
	return !CooldownEnd || *CooldownEnd <= GetWorld()->GetTimeSeconds();
}

float USKSkillComponent::GetSkillCooldownRemaining(const FName& SkillID) const
{
	if(const float* CooldownEnd = SkillCooldowns.Find(SkillID))
	{
		float RemainingTime = *CooldownEnd - GetWorld()->GetTimeSeconds();
		return FMath::Max(RemainingTime, 0.0f);
	}

	return 0.0f;
}
