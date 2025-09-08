// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Skill/SKSkillData.h"
#include "SKSkillAnimNotify_Trigger.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSkillNotifyTriggered, FName, NotifyName);

class ASKBaseCharacter;
class ASKProjectileActor;

UCLASS()
class SKILLMAKER_API USKSkillAnimNotify_Trigger : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	virtual FString GetNotifyName_Implementation() const override;

protected:
	/** 광역 효과 적용 */
	void ApplyAOEEffect(ASKBaseCharacter* Character, const FSKSkillData& SkillData);

	/** 발사체 생성 */
	void SpawnProjectile(ASKBaseCharacter* Character, const TSubclassOf<ASKProjectileActor> ProjectileClass);
	
public:
	UPROPERTY(BlueprintAssignable, Category = "Skill")
	FOnSkillNotifyTriggered OnSkillNotifyTriggered;

	/** 애님 노티파이 트리거 이름 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
	FName NotifyTriggerName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
	TArray<FStatusEffectData> StatusEffects;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
	float StatusDuration;
	
	/** 적용할 상태이상 ID */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
	TArray<FName> AppliedBuffs;
};
