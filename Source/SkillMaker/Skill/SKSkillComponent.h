// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SKSkillData.h"
#include "SKSkillComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SKILLMAKER_API USKSkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USKSkillComponent();

	void SetSkillDataInMap(const FSKSkillData& SkillData);
	
	void SetSkillMapInComponent(const TMap<FName, FSKSkillData>& SkillDataMap);
	
	/** 클라이언트가 서버에 스킬 사용 요청을 보내는 함수 */
	UFUNCTION(BlueprintCallable, Category = "Skill")
	void ClientRequestUseSkill(const FName& SkillID);
	
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category = "Skill")
	void ServerUseSkill(const FName& SkillID);

	UFUNCTION(NetMulticast, Reliable, Category = "Skill")
	void MulticastExecuteSkill(const FSKSkillData& SkillData);
	
	UFUNCTION(BlueprintPure, Category = "Skill")
	bool IsSkillAvailable(const FName& SkillID) const;

	UFUNCTION(BlueprintPure, Category = "Skill")
	float GetSkillCooldownRemaining(const FName& SkillID) const;

protected:
	/** 스킬 실행 */
	void ExecuteSkill(const FSKSkillData& SkillData);

	/** 쿨다운 적용 */
	void ApplyCooldown(const FName& SkillID, float CooldownTime);

	/** 타이머가 끝나면 쿨다운 제거 */
	void ClearCooldown(FName SkillID);
	
	/** 서버가 클라이언트의 요청을 검증하는 함수 */
	bool ServerUseSkill_Validate(const FName& SkillID);

	/** 서버에서 실행될 함수의 실제 구현 */
	void ServerUseSkill_Implementation(const FName& SkillID);

	/** 멀티캐스트 함수의 실제 구현 */
	void MulticastExecuteSkill_Implementation(const FSKSkillData& SkillData);

	UPROPERTY()
	TMap<FName, float> SkillCooldowns;

	UPROPERTY()
	TMap<FName, FTimerHandle> CooldownTimers;

	UPROPERTY()
	TMap<FName, FSKSkillData> SkillMap;
};
