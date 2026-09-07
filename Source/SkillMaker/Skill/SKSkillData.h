#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"
#include "SKSkillData.generated.h"

class ASKProjectileActor;
class ASKBaseCharacter;
class UAnimMontage;
class UBlendSpace;

UENUM(BlueprintType)
enum class ESkillType : uint8
{
	Attack UMETA(DisplayName = "Attack"),
	Buff UMETA(DisplayName = "Buff"),
	Debuff UMETA(DisplayName = "Debuff")
};

UENUM(BlueprintType)
enum class ESKTargetingType : uint8
{
	NonTarget UMETA(DisplayName = "논타겟"),
	TargetActor UMETA(DisplayName = "타겟팅")
};

UENUM(BlueprintType)
enum class EStatusEffect : uint8
{
	None        UMETA(DisplayName = "None"),
	Stun        UMETA(DisplayName = "Stun"),       // 이동 및 행동 불가
	Slow        UMETA(DisplayName = "Slow"),       // 이동 속도 감소
	Burn        UMETA(DisplayName = "Burn"),       // 지속 피해 (화상)
	Freeze      UMETA(DisplayName = "Freeze"),     // 일정 시간 동안 얼음 상태
	Poison      UMETA(DisplayName = "Poison"),     // 지속 피해 (중독)
	Shock       UMETA(DisplayName = "Shock"),      // 감전 효과 (일시적 기절)
	Curse       UMETA(DisplayName = "Curse")       // 디버프 효과 (방어력 감소 등)
};

ENUM_RANGE_BY_COUNT(EStatusEffect, EStatusEffect::Curse)

USTRUCT(BlueprintType)
struct FStatusEffectData
{
	GENERATED_BODY()

public:
	/** 적용할 상태이상 유형 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category = "StatusEffect")
	EStatusEffect EffectType;

	/** 상태이상 지속 시간 (초) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category = "StatusEffect")
	float Duration;

	/** 초당 지속 피해량 (화상, 중독 등의 경우) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category = "StatusEffect")
	float DamagePerSecond;

	/** 스택 가능 여부 (같은 상태이상 중첩 가능 여부) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category = "StatusEffect")
	bool bCanStack;

	/** 상태이상의 최대 스택 수 (0이면 무제한) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category = "StatusEffect")
	int32 MaxStack;

	FStatusEffectData() 
		: EffectType(EStatusEffect::None)
		, Duration(0.0f)
		, DamagePerSecond(0.0f)
		, bCanStack(false)
		, MaxStack(1) 
	{}

	FStatusEffectData(EStatusEffect InEffectType, float InDuration, float InDamagePerSecond, bool InCanStack = false, int32 InMaxStack = 1)
		: EffectType(InEffectType)
		, Duration(InDuration)
		, DamagePerSecond(InDamagePerSecond)
		, bCanStack(InCanStack)
		, MaxStack(InMaxStack)
	{}
};

USTRUCT(BlueprintType)
struct FSKSkillData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category = "Skill")
	FName SkillID;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category = "Skill")
	FString SkillName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category = "Skill")
	ESkillType SkillType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category = "Skill")
	ESKTargetingType TargetingType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category = "Element", meta = (Categories = "Element"))
	FGameplayTag ElementTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category = "Skill", meta = (Categories = "Weapon"))
	FGameplayTag WeaponTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category = "Skill")
	TSoftObjectPtr<UAnimMontage> SkillMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category = "Skill")
	float SkillDuration;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category = "Skill")
	bool bCanMoveWhileChanneling;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category = "Skill")
	float CooldownTime;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category = "Skill")
	float Cost;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category = "Skill")
	float DamageValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category = "Skill")
	TArray<FName> EffectNotifyNames;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category = "Skill")
	TArray<FStatusEffectData> StatusEffects;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category = "Skill")
	bool bAffectEnemies;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category = "Skill")
	bool bAffectAllies;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category = "Skill")
	TSoftClassPtr<ASKProjectileActor> ProjectileActor;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category = "Effect")
	FName NotifyName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category = "Skill")
	float MinRange;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category = "Skill")
	float MaxRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category = "Skill")
	bool bUseMoveBlendSpace;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category = "Skill")
	TSoftObjectPtr<UBlendSpace> MoveSkillBlendSpace;

	FSKSkillData()
		: SkillType(ESkillType::Attack)
		, TargetingType(ESKTargetingType::NonTarget)
		, SkillDuration(0.0f)
		, bCanMoveWhileChanneling(false)
		, CooldownTime(0.0f)
		, Cost(0.0f)
		, DamageValue(0.0f)
		, bAffectEnemies(true)
		, bAffectAllies(false)
		, MinRange(0.0f)
		, MaxRange(0.0f)
		, bUseMoveBlendSpace(false)
	{}
};

/**
 * 발사 순간의 스킬 전투 정보입니다.
 * 애니메이션과 발사체 표현 리소스는 제외하고, 충돌 결과 계산에 필요한 값과 공격자를 복사합니다.
 */
USTRUCT(BlueprintType)
struct FSKSkillImpactData
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill Impact")
	FName SkillID;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill Impact")
	FString SkillName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill Impact")
	ESkillType SkillType = ESkillType::Attack;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill Impact")
	ESKTargetingType TargetingType = ESKTargetingType::NonTarget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill Impact")
	FGameplayTag ElementTag;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill Impact")
	FGameplayTag WeaponTag;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill Impact")
	float SkillDuration = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill Impact")
	bool bCanMoveWhileChanneling = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill Impact")
	float CooldownTime = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill Impact")
	float Cost = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill Impact")
	float DamageValue = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill Impact")
	TArray<FStatusEffectData> StatusEffects;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill Impact")
	bool bAffectEnemies = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill Impact")
	bool bAffectAllies = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill Impact")
	float MinRange = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill Impact")
	float MaxRange = 0.0f;

	/** 이 공격을 발생시킨 캐릭터 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Transient, Category = "Skill Impact")
	TObjectPtr<ASKBaseCharacter> SourceCharacter;

	FSKSkillImpactData() = default;

	FSKSkillImpactData(const FSKSkillData& SkillData, ASKBaseCharacter* InSourceCharacter)
		: SkillID(SkillData.SkillID)
		, SkillName(SkillData.SkillName)
		, SkillType(SkillData.SkillType)
		, TargetingType(SkillData.TargetingType)
		, ElementTag(SkillData.ElementTag)
		, WeaponTag(SkillData.WeaponTag)
		, SkillDuration(SkillData.SkillDuration)
		, bCanMoveWhileChanneling(SkillData.bCanMoveWhileChanneling)
		, CooldownTime(SkillData.CooldownTime)
		, Cost(SkillData.Cost)
		, DamageValue(SkillData.DamageValue)
		, StatusEffects(SkillData.StatusEffects)
		, bAffectEnemies(SkillData.bAffectEnemies)
		, bAffectAllies(SkillData.bAffectAllies)
		, MinRange(SkillData.MinRange)
		, MaxRange(SkillData.MaxRange)
		, SourceCharacter(InSourceCharacter)
	{}
};
