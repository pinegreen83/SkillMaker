#pragma once

#include "CoreMinimal.h"
#include "SKSkillData.generated.h"

USTRUCT(BlueprintType)
struct FSKSkillData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	FString SkillName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	FString WeaponType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	TObjectPtr<UAnimMontage> SkillMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	float SkillDuration;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	TArray<FName> EffectNotifyNames;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	TArray<FName> SoundNotifyNames;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	TMap<FName, FName> EffectSoundMapping;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	float CooldownTime;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	float Cost;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	float MinRange;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	float MaxRange;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	float DamageValue;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	FString HitAreaType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	TArray<FName> AppliedBuffs;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	TMap<FName, float> NotifyEffectTimings;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	int32 RowNum;

	FSKSkillData()
		: SkillDuration(0.0f)
		, CooldownTime(0.0f)
		, Cost(0.0f)
		, MinRange(0.0f)
		, MaxRange(0.0f)
		, DamageValue(0.0f)
		, RowNum(-1)
	{}
};
