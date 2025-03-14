#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SKSkillData.generated.h"

USTRUCT(BlueprintType)
struct FSoundEffectMapping
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	FName NotifyName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	FName EffectOrSound;

	FSoundEffectMapping() :
		NotifyName(NAME_None),
		EffectOrSound(NAME_None)
	{}
};

UENUM(BlueprintType)
enum class EStatusEffect : uint8
{
	None UMETA(DisplayName = "None"),
	Stun UMETA(DisplayName = "Stun"),
	Slow UMETA(DisplayName = "Slow"),
	Burn UMETA(DisplayName = "Burn"),
	Freeze UMETA(DisplayName = "Freeze"),
	Poison UMETA(DisplayName = "Poison")
};

USTRUCT(BlueprintType)
struct FStatusEffectData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatusEffect")
	EStatusEffect EffectType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatusEffect")
	float Duration;

	FStatusEffectData() : 
		EffectType(EStatusEffect::None),
		Duration(0.0f)
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
	FString WeaponType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	TObjectPtr<UAnimMontage> SkillMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	float SkillDuration;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	bool bCanMoveWhileCasting;
	
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
	TArray<FSoundEffectMapping> EffectSoundMappings;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	float MinRange;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	float MaxRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	bool bUseMoveBlendSpace;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	TObjectPtr<UBlendSpace> MoveSkillBlendSpace;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	TSubclassOf<AActor> ProjectileClass;

	FSKSkillData()
		: SkillDuration(0.0f)
		, bCanMoveWhileCasting(false)
		, CooldownTime(0.0f)
		, Cost(0.0f)
		, DamageValue(0.0f)
		, bAffectEnemies(true)
		, bAffectAllies(false)
		, MinRange(0.0f)
		, MaxRange(0.0f)
		, bUseMoveBlendSpace(false)
		, ProjectileClass(nullptr)
	{}
};
