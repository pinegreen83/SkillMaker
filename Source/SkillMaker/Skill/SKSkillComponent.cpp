// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/SKSkillComponent.h"
#include "SKSkillManager.h"
#include "GameFramework/Character.h"
#include "Animation/AnimInstance.h"
#include "Character/SKPlayerCharacter.h"
#include "Logging/SKLogSkillMakerMacro.h"

// Sets default values for this component's properties
USKSkillComponent::USKSkillComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);

	SkillMap.Empty();
}

void USKSkillComponent::SetSkillDataInMap(const FSKSkillData& SkillData)
{
	SkillMap.Add(SkillData.SkillID, SkillData);
}

void USKSkillComponent::SetSkillMapInComponent(const TMap<FName, FSKSkillData>& SkillDataMap)
{
	SkillMap = SkillDataMap;
}

// Component에서 스킬 실행
void USKSkillComponent::ClientRequestUseSkill(const FName& SkillID)
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));
	
	// 서버에서 스킬 실행
	if(GetOwner()->HasAuthority())
	{
		SK_LOG(LogSkillMaker, Log, TEXT("서버에서 스킬 실행"));
		if(const FSKSkillData* SkillData = SkillMap.Find(SkillID))
		{
			ExecuteSkill(*SkillData);
		}
	}
	// 클라이언트에서 요청
	else
	{
		SK_LOG(LogSkillMaker, Log, TEXT("클라이언트에서 서버에 요청"));
		ServerUseSkill(SkillID);
	}
}

bool USKSkillComponent::ServerUseSkill_Validate(const FName& SkillID)
{
	if(SkillMap.Contains(SkillID) || SkillCooldowns.Contains(SkillID))
		return SkillID.IsValid();

	return false;
}

void USKSkillComponent::ServerUseSkill_Implementation(const FName& SkillID)
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));
	
	if(!IsSkillAvailable(SkillID))
	{
		SK_LOG(LogSkillMaker, Warning, TEXT("스킬 사용 불가(쿨다운 중) : %s"), *SkillID.ToString());
		return;
	}

	if(const FSKSkillData* SkillData = SkillMap.Find(SkillID))
	{
		ExecuteSkill(*SkillData);
	}
	else
	{
		SK_LOG(LogSkillMaker, Error, TEXT("스킬 데이터를 찾을 수 없음 : %s"), *SkillID.ToString());
	}
}

void USKSkillComponent::MulticastExecuteSkill_Implementation(const FSKSkillData& SkillData)
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));
	
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if(!OwnerCharacter || !SkillData.SkillMontage)
	{
		SK_LOG(LogSkillMaker, Error, TEXT("스킬 실행 불가 : 캐릭터 또는 애니메이션 몽타주 없음."));
		return;
	}

	if(UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance())
	{
		if (ASKPlayerCharacter* PlayerCharacter = Cast<ASKPlayerCharacter>(OwnerCharacter))
		{
			const TOptional<FSKSkillData> TempSkillData;
			PlayerCharacter->SetCurrentSkillData(TempSkillData);
		
			AnimInstance->Montage_Play(SkillData.SkillMontage);
			SK_LOG(LogSkillMaker, Log, TEXT("애니메이션 실행 : %s"), *SkillData.SkillName);
		}
	}
}

void USKSkillComponent::ExecuteSkill(const FSKSkillData& SkillData)
{
	MulticastExecuteSkill(SkillData);

	ApplyCooldown(SkillData.SkillID, SkillData.CooldownTime);
}

void USKSkillComponent::ApplyCooldown(const FName& SkillID, float CooldownTime)
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));
	
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
		SK_LOG(LogSkillMaker, Log, TEXT("쿨다운 적용 : %s, %f초 후 해제"), *SkillID.ToString(), CooldownTime);
	}
}

void USKSkillComponent::ClearCooldown(FName SkillID)
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));
	
	SkillCooldowns.Remove(SkillID);
	CooldownTimers.Remove(SkillID);
	
	SK_LOG(LogSkillMaker, Log, TEXT("쿨다운 해제 됨 : %s"), *SkillID.ToString());
}

bool USKSkillComponent::IsSkillAvailable(const FName& SkillID) const
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));

	const float* CooldownEnd = SkillCooldowns.Find(SkillID);
	return !CooldownEnd || *CooldownEnd <= GetWorld()->GetTimeSeconds();
}

float USKSkillComponent::GetSkillCooldownRemaining(const FName& SkillID) const
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));
	
	if(const float* CooldownEnd = SkillCooldowns.Find(SkillID))
	{
		float RemainingTime = *CooldownEnd - GetWorld()->GetTimeSeconds();
		return FMath::Max(RemainingTime, 0.0f);
	}

	return 0.0f;
}
