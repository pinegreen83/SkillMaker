#pragma once

#include "CoreMinimal.h"
#include "FSkillData.generated.h"

USTRUCT(BlueprintType)
struct FEffectTiming
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillEffect")
	float TriggerTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillEffect")
	TSubclassOf<AActor> EffectActorClass;
};

USTRUCT(BlueprintType)
struct FSkillData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillData")
	FName SkillName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillData")
	TObjectPtr<class UAnimMontage> SkillMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillData")
	TArray<FEffectTiming> EffectTimings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillData")
	float Damage = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillData")
	float CoolDown = 0.0f;
};
