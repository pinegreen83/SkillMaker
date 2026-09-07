// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/SKCombatComponent.h"
#include "Character/SKBaseCharacter.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Logging/SKLogSkillMakerMacro.h"
#include "TimerManager.h"

USKCombatComponent::USKCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);

	MaxHealth = 100.0f;
	CurrentHealth = MaxHealth;
	OwnerCharacter = nullptr;
}

// Called when the game starts
void USKCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	
	OwnerCharacter = Cast<ASKBaseCharacter>(GetOwner());
}

void USKCombatComponent::TakeDamage(float DamageAmount)
{
	if(GetOwnerRole() < ROLE_Authority)
	{
		SK_LOG(LogSkillMaker, Log, TEXT("클라이언트 피해 요청을 서버로 전달: Target=%s / Damage=%.2f"),
			*GetNameSafe(GetOwner()), DamageAmount);
		ServerTakeDamage(DamageAmount);
		return;
	}

	const float HealthBefore = CurrentHealth;
	CurrentHealth = FMath::Clamp(CurrentHealth - DamageAmount, 0.0f, MaxHealth);
	SK_LOG(LogSkillMaker, Log,
		TEXT("체력 감소: Target=%s / Damage=%.2f / HealthBefore=%.2f / HealthAfter=%.2f / MaxHealth=%.2f"),
		*GetNameSafe(GetOwner()), DamageAmount, HealthBefore, CurrentHealth, MaxHealth);

	if(CurrentHealth <= 0.0f)
	{
		HandleDeath();
	}
}

void USKCombatComponent::ApplySkillImpact(const FSKSkillImpactData& ImpactData)
{
	SK_LOG(LogSkillMaker, Log,
		TEXT("스킬 충격 수신: Source=%s / Target=%s / SkillID=%s / SkillName=%s / BaseDamage=%.2f / Element=%s / StatusEffectCount=%d"),
		*GetNameSafe(ImpactData.SourceCharacter), *GetNameSafe(GetOwner()), *ImpactData.SkillID.ToString(),
		*ImpactData.SkillName, ImpactData.DamageValue, *ImpactData.ElementTag.ToString(), ImpactData.StatusEffects.Num());
	SK_LOG(LogSkillMaker, Log,
		TEXT("스킬 충격 상세 수신: SkillType=%d / TargetingType=%d / Weapon=%s / Duration=%.2f / CanMove=%s / Cooldown=%.2f / Cost=%.2f / Range=%.2f~%.2f / AffectEnemies=%s / AffectAllies=%s"),
		static_cast<int32>(ImpactData.SkillType), static_cast<int32>(ImpactData.TargetingType),
		*ImpactData.WeaponTag.ToString(), ImpactData.SkillDuration,
		ImpactData.bCanMoveWhileChanneling ? TEXT("True") : TEXT("False"), ImpactData.CooldownTime,
		ImpactData.Cost, ImpactData.MinRange, ImpactData.MaxRange,
		ImpactData.bAffectEnemies ? TEXT("True") : TEXT("False"),
		ImpactData.bAffectAllies ? TEXT("True") : TEXT("False"));

	if (GetOwnerRole() < ROLE_Authority)
	{
		SK_LOG(LogSkillMaker, Warning,
			TEXT("스킬 충격 적용 거부: 대상에 서버 권한 없음. Target=%s / SkillID=%s"),
			*GetNameSafe(GetOwner()), *ImpactData.SkillID.ToString());
		return;
	}

	if (!OwnerCharacter || !ImpactData.SourceCharacter || ImpactData.SkillID.IsNone())
	{
		SK_LOG(LogSkillMaker, Error,
			TEXT("스킬 충격 적용 실패: OwnerCharacter=%s / Source=%s / SkillID=%s"),
			*GetNameSafe(OwnerCharacter), *GetNameSafe(ImpactData.SourceCharacter), *ImpactData.SkillID.ToString());
		return;
	}

	if (OwnerCharacter == ImpactData.SourceCharacter)
	{
		SK_LOG(LogSkillMaker, Warning, TEXT("자기 자신에 대한 스킬 충격 차단: Character=%s / SkillID=%s"),
			*OwnerCharacter->GetName(), *ImpactData.SkillID.ToString());
		return;
	}

	const float FinalDamage = CalculateFinalDamage(ImpactData);
	SK_LOG(LogSkillMaker, Log,
		TEXT("최종 피해 계산 완료: Source=%s / Target=%s / SkillID=%s / BaseDamage=%.2f / FinalDamage=%.2f"),
		*ImpactData.SourceCharacter->GetName(), *OwnerCharacter->GetName(), *ImpactData.SkillID.ToString(),
		ImpactData.DamageValue, FinalDamage);

	for (const FStatusEffectData& StatusEffect : ImpactData.StatusEffects)
	{
		SK_LOG(LogSkillMaker, Log,
			TEXT("상태이상 전투 정보 수신: SkillID=%s / Type=%d / Duration=%.2f / DPS=%.2f / CanStack=%s / MaxStack=%d"),
			*ImpactData.SkillID.ToString(), static_cast<int32>(StatusEffect.EffectType), StatusEffect.Duration,
			StatusEffect.DamagePerSecond, StatusEffect.bCanStack ? TEXT("True") : TEXT("False"), StatusEffect.MaxStack);
	}

	if (FinalDamage <= 0.0f)
	{
		SK_LOG(LogSkillMaker, Warning, TEXT("적용할 피해가 없어 체력 변경 생략: SkillID=%s / FinalDamage=%.2f"),
			*ImpactData.SkillID.ToString(), FinalDamage);
		return;
	}

	TakeDamage(FinalDamage);
}

float USKCombatComponent::CalculateFinalDamage(const FSKSkillImpactData& ImpactData) const
{
	// 속성 공격력, 저항, 무기 보정 등은 이 함수에 단계적으로 추가합니다.
	return FMath::Max(0.0f, ImpactData.DamageValue);
}

bool USKCombatComponent::ServerTakeDamage_Validate(float DamageAmount)
{
	return DamageAmount > 0;
}

void USKCombatComponent::ServerTakeDamage_Implementation(float DamageAmount)
{
	TakeDamage(DamageAmount);
}

void USKCombatComponent::ApplyStatusEffect(EStatusEffect EffectType, float Duration)
{
	if(GetOwnerRole() < ROLE_Authority)
	{
		ServerApplyStatusEffect(EffectType, Duration);
		return;
	}

	MulticastApplyStatusEffect(EffectType, Duration);
}

bool USKCombatComponent::ServerApplyStatusEffect_Validate(EStatusEffect EffectType, float Duration)
{
	return Duration > 0.0f;
}

void USKCombatComponent::ServerApplyStatusEffect_Implementation(EStatusEffect EffectType, float Duration)
{
	MulticastApplyStatusEffect(EffectType, Duration);
}

void USKCombatComponent::MulticastApplyStatusEffect_Implementation(EStatusEffect EffectType, float Duration)
{
	if(!OwnerCharacter)
		return;

	switch(EffectType)
	{
	case EStatusEffect::Stun:
		UE_LOG(LogTemp, Log, TEXT("전투 컴포넌트 : 스턴 상태 적용"));
		OwnerCharacter->GetCharacterMovement()->DisableMovement();
		break;
		
	case EStatusEffect::Slow:
		UE_LOG(LogTemp, Log, TEXT("전투 컴포넌트 : 이동 속도 감소"));
		OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed *= 0.5f;
		break;

	case EStatusEffect::Burn:
		UE_LOG(LogTemp, Log, TEXT("전투 컴포넌트 : 화상 효과 적용"));
		GetWorld()->GetTimerManager().SetTimer(StatusEffectTimers[EffectType], [this]()
		{
			TakeDamage(5.0f);
		}, 1.0f, true);
		break;

	case EStatusEffect::Freeze:
		UE_LOG(LogTemp, Log, TEXT("전투 컴포넌트 : 빙결 효과 적용"));
		OwnerCharacter->GetCharacterMovement()->DisableMovement();
		break;

	case EStatusEffect::Poison:
		UE_LOG(LogTemp, Log, TEXT("전투 컴포넌트 : 중독 효과 적용"));
		GetWorld()->GetTimerManager().SetTimer(StatusEffectTimers[EffectType], [this]()
		{
			TakeDamage(5.0f);
		}, 1.0f, true);
		break;
		
	default:
		break;
	}

	GetWorld()->GetTimerManager().SetTimer(StatusEffectTimers[EffectType], [this, EffectType]()
	{
		if (!OwnerCharacter) return;

		if (EffectType == EStatusEffect::Stun || EffectType == EStatusEffect::Freeze)
		{
			OwnerCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		}
		else if (EffectType == EStatusEffect::Slow)
		{
			OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed *= 2.0f;
		}

		if (EffectType == EStatusEffect::Burn)
		{
			GetWorld()->GetTimerManager().ClearTimer(StatusEffectTimers[EffectType]);
		}

		UE_LOG(LogTemp, Log, TEXT("전투 컴포넌트: 상태 이상 해제"));

	}, Duration, false);
}

void USKCombatComponent::HandleDeath()
{
	if (!OwnerCharacter) return;
	SK_LOG(LogSkillMaker, Log, TEXT("캐릭터 사망: %s"), *OwnerCharacter->GetName());
	OwnerCharacter->Destroy();
}

void USKCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(USKCombatComponent, MaxHealth);
	DOREPLIFETIME(USKCombatComponent, CurrentHealth);
}
