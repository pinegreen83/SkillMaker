#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SKProjectileActor.h"
#include "SKSkillData.generated.h"

UENUM(BlueprintType)
enum class ESkillType : uint8
{
	Attack UMETA(DisplayName = "Attack"),
	Buff UMETA(DisplayName = "Buff"),
	Debuff UMETA(DisplayName = "Debuff")
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatusEffect")
	EStatusEffect EffectType;

	/** 상태이상 지속 시간 (초) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatusEffect")
	float Duration;

	/** 초당 지속 피해량 (화상, 중독 등의 경우) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatusEffect")
	float DamagePerSecond;

	/** 스택 가능 여부 (같은 상태이상 중첩 가능 여부) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatusEffect")
	bool bCanStack;

	/** 상태이상의 최대 스택 수 (0이면 무제한) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatusEffect")
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	FName SkillID;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	FString SkillName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	ESkillType SkillType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	FString WeaponType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	TObjectPtr<UAnimMontage> SkillMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	float SkillDuration;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	bool bCanMoveWhileChanneling;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	float CooldownTime;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	float Cost;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	float DamageValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	TArray<FName> EffectNotifyNames;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	TArray<FStatusEffectData> StatusEffects;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	bool bAffectEnemies;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	bool bAffectAllies;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	TSubclassOf<ASKProjectileActor> ProjectileActor;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	FName NotifyName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	float MinRange;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	float MaxRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	bool bUseMoveBlendSpace;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	TObjectPtr<UBlendSpace> MoveSkillBlendSpace;

	FSKSkillData()
		: SkillType(ESkillType::Attack)
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
